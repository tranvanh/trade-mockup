#include "ClientApplication.h"
#include <TradeCommon/Protocol.h>
#include <TradeCommon/Validation.h>
#include <ftxui/component/component.hpp>
#include <ftxui/component/component_base.hpp>
#include <ftxui/component/event.hpp>
#include <ftxui/component/screen_interactive.hpp>
#include <ftxui/dom/elements.hpp>
#include <iostream>
#include <chrono>
#include <sstream>
#include <thread>

namespace {
    constexpr unsigned short DEFAULT_PORT = 8080;
    constexpr const char*    DEFAULT_HOST = "127.0.0.1";

    std::string joinEndpoint(const std::string& host, const unsigned short port) {
        return host + ":" + std::to_string(port);
    }
}

void ClientApplication::run() {
    Application::run();

    switch (mOptions.mode) {
    case TradeCommon::ClientLaunchMode::Tui:
        runTui();
        break;
    case TradeCommon::ClientLaunchMode::LegacyManual:
        runLegacyManual();
        break;
    case TradeCommon::ClientLaunchMode::LegacySimulation:
        runLegacySimulation();
        break;
    }

    disconnectFromServer();
    stop();
}

bool ClientApplication::connectToServer(const std::string& host, const unsigned short port) {
    disconnectFromServer();

    if (host.empty()) {
        pushStatus("Host must not be empty.");
        return false;
    }

    auto client          = std::make_unique<TradeCommon::TcpClient>();
    client->onMessage    = [this](const std::string& message) { handleServerMessage(message); };
    client->onDisconnect = [this]() {
        mConnected = false;
        mGenerator.stop();
        pushStatus("Connection lost.");
    };

    if (!client->connect(host, port)) {
        pushStatus("Failed to connect to " + joinEndpoint(host, port) + '.');
        return false;
    }

    {
        std::lock_guard<std::mutex> lock(mClientMutex);
        mClient = std::move(client);
    }

    mTradeHistory.clear();
    mConnected = true;
    pushStatus("Connected to " + joinEndpoint(host, port) + '.');
    return true;
}

void ClientApplication::disconnectFromServer(const std::string& reason) {
    mGenerator.stop();

    std::unique_ptr<TradeCommon::TcpClient> client;
    {
        std::lock_guard<std::mutex> lock(mClientMutex);
        client = std::move(mClient);
    }

    const bool wasConnected = mConnected.exchange(false);
    if (client) {
        client->stop();
    }

    if (!reason.empty()) {
        pushStatus(reason);
    } else if (wasConnected) {
        pushStatus("Disconnected.");
    }
}

void ClientApplication::handleServerMessage(const std::string& message) {
    try {
        const auto envelope = TradeCommon::parseEnvelope(message);
        switch (envelope.type) {
        case TradeCommon::MessageType::Trade:
            pushTrade(TradeCommon::formatTradeLine(std::get<TradeCommon::TradePayload>(envelope.payload)));
            break;
        case TradeCommon::MessageType::Error:
            pushStatus("Server error: " + std::get<TradeCommon::ErrorPayload>(envelope.payload).message);
            break;
        case TradeCommon::MessageType::SubmitOrder:
            pushStatus("Received unexpected submit_order message from server.");
            break;
        }
    } catch (const std::exception& ex) {
        pushStatus(std::string("Invalid server message: ") + ex.what());
    }
}

void ClientApplication::runLegacyManual() {
    if (!connectToServer(DEFAULT_HOST, DEFAULT_PORT)) {
        return;
    }
    processUserInputs(mOptions.clientId);
}

void ClientApplication::runLegacySimulation() {
    if (!connectToServer(DEFAULT_HOST, DEFAULT_PORT)) {
        return;
    }

    pushStatus("Starting simulation mode.");
    mGenerator.start(std::nullopt);
    while (isRunning && mConnected.load()) {
        std::this_thread::sleep_for(std::chrono::milliseconds(250));
    }
}

void ClientApplication::runTui() {
    using namespace ftxui;

    ScreenInteractive screen = ScreenInteractive::TerminalOutput();
    {
        std::lock_guard<std::mutex> lock(mNotifierMutex);
        mUiNotifier = [&screen]() { screen.PostEvent(Event::Custom); };
    }

    std::string              clientIdInput;
    std::string              hostInput  = DEFAULT_HOST;
    std::string              portInput  = std::to_string(DEFAULT_PORT);
    std::string              priceInput;
    std::string              volumeInput;
    int                      selectedMode   = 0;
    int                      activeScreen   = 0;
    int                      sessionClientId = 0;
    std::vector<std::string> modes = {"Manual", "Simulation"};

    auto renderHistoryWindow = [](const std::string& title, const std::vector<std::string>& items) {
        Elements lines;
        if (items.empty()) {
            lines.push_back(text("No data yet.") | dim);
        } else {
            for (const auto& item : items) {
                lines.push_back(paragraph(item));
            }
        }

        return window(text(title), vbox(std::move(lines)) | yframe | flex);
    };

    auto connectAction = [&]() {
        const auto parsedClientId = TradeCommon::tryParseInt(clientIdInput);
        if (!parsedClientId) {
            pushStatus("Client id must be a positive integer.");
            return;
        }

        if (const auto error = TradeCommon::validateClientId(*parsedClientId)) {
            pushStatus(*error);
            return;
        }

        const auto parsedPort = TradeCommon::tryParsePort(portInput);
        if (!parsedPort) {
            pushStatus("Port must be between 1 and 65535.");
            return;
        }

        sessionClientId = *parsedClientId;
        if (!connectToServer(hostInput, *parsedPort)) {
            return;
        }

        priceInput.clear();
        volumeInput.clear();
        activeScreen = selectedMode == 0 ? 1 : 2;
        pushStatus(selectedMode == 0 ? "Manual session ready." : "Simulation session ready.");
    };

    auto startSimulation = [&]() {
        if (!mConnected) {
            pushStatus("Connect before starting simulation.");
            return;
        }
        if (mGenerator.isActive()) {
            pushStatus("Simulation is already running.");
            return;
        }
        mGenerator.start(sessionClientId);
        pushStatus("Simulation started.");
    };

    auto stopSimulation = [&]() {
        if (!mGenerator.isActive()) {
            pushStatus("Simulation is not running.");
            return;
        }
        mGenerator.stop();
        pushStatus("Simulation stopped.");
    };

    auto submitOrderAction = [&](const TradeCore::OrderType side) {
        const auto parsedPrice  = TradeCommon::tryParseInt(priceInput);
        const auto parsedVolume = TradeCommon::tryParseInt(volumeInput);
        if (!parsedPrice || !parsedVolume) {
            pushStatus("Price and volume must be positive integers.");
            return;
        }

        if (!submitOrder(sessionClientId, side, *parsedPrice, *parsedVolume)) {
            return;
        }

        priceInput.clear();
        volumeInput.clear();
    };

    Component clientIdField = Input(&clientIdInput, "Client id");
    Component hostField     = Input(&hostInput, "Host");
    Component portField     = Input(&portInput, "Port");
    Component modeSelector  = Radiobox(&modes, &selectedMode);
    Component connectButton = Button("Connect", connectAction);
    Component exitButton    = Button("Quit", screen.ExitLoopClosure());

    Component priceField       = Input(&priceInput, "Price");
    Component volumeField      = Input(&volumeInput, "Volume");
    Component buyButton        = Button("Buy", [&]() { submitOrderAction(TradeCore::OrderType::BUY); });
    Component sellButton       = Button("Sell", [&]() { submitOrderAction(TradeCore::OrderType::SELL); });
    Component disconnectButton = Button("Disconnect", [&]() {
        disconnectFromServer();
        activeScreen = 0;
    });
    Component quitButton = Button("Quit", [&]() {
        disconnectFromServer();
        screen.ExitLoopClosure()();
    });

    Component startSimulationButton = Button("Start Simulation", startSimulation);
    Component stopSimulationButton  = Button("Stop Simulation", stopSimulation);
    Component simulationDisconnectButton = Button("Disconnect", [&]() {
        disconnectFromServer();
        activeScreen = 0;
    });
    Component simulationQuitButton = Button("Quit", [&]() {
        disconnectFromServer();
        screen.ExitLoopClosure()();
    });

    Component startContainer = Container::Vertical(
        {clientIdField, hostField, portField, modeSelector, connectButton, exitButton});
    Component manualContainer =
        Container::Vertical({priceField, volumeField, buyButton, sellButton, disconnectButton, quitButton});
    Component simulationContainer = Container::Vertical(
        {startSimulationButton, stopSimulationButton, simulationDisconnectButton, simulationQuitButton});

    Component startScreen = Renderer(startContainer, [&] {
        return vbox({
                   text("Trading Client") | bold,
                   text("No-arg launch opens the terminal UI.") | dim,
                   separator(),
                   text("Client ID"),
                   clientIdField->Render(),
                   text("Host"),
                   hostField->Render(),
                   text("Port"),
                   portField->Render(),
                   text("Mode"),
                   modeSelector->Render(),
                   hbox({connectButton->Render(), filler(), exitButton->Render()}),
                   separator(),
                   renderHistoryWindow("Status", statusSnapshot()) | size(HEIGHT, LESS_THAN, 12),
               })
            | border;
    });

    Component manualScreen = Renderer(manualContainer, [&] {
        return vbox({
                   text("Manual Trading Session") | bold,
                   text("Client ID: " + std::to_string(sessionClientId)),
                   text(std::string("Connection: ") + (mConnected.load() ? "connected" : "disconnected")),
                   separator(),
                   hbox({
                       vbox({
                           text("Price"),
                           priceField->Render(),
                           text("Volume"),
                           volumeField->Render(),
                           hbox({buyButton->Render(), filler(), sellButton->Render()}),
                           separator(),
                           hbox({disconnectButton->Render(), filler(), quitButton->Render()}),
                       }) | flex,
                       separator(),
                       renderHistoryWindow("Recent Trades", tradeSnapshot()) | flex,
                   }) | flex,
                   separator(),
                   renderHistoryWindow("Status", statusSnapshot()) | size(HEIGHT, LESS_THAN, 12),
               })
            | border;
    });

    Component simulationScreen = Renderer(simulationContainer, [&] {
        return vbox({
                   text("Simulation Session") | bold,
                   text("Client ID: " + std::to_string(sessionClientId)),
                   text(std::string("Connection: ") + (mConnected.load() ? "connected" : "disconnected")),
                   text(std::string("Simulation: ") + (mGenerator.isActive() ? "running" : "stopped")),
                   separator(),
                   hbox({
                       vbox({
                           startSimulationButton->Render(),
                           stopSimulationButton->Render(),
                           separator(),
                           simulationDisconnectButton->Render(),
                           simulationQuitButton->Render(),
                       }) | flex,
                       separator(),
                       renderHistoryWindow("Recent Trades", tradeSnapshot()) | flex,
                   }) | flex,
                   separator(),
                   renderHistoryWindow("Status", statusSnapshot()) | size(HEIGHT, LESS_THAN, 12),
               })
            | border;
    });

    Component root = Container::Tab({startScreen, manualScreen, simulationScreen}, &activeScreen);
    screen.Loop(root);

    {
        std::lock_guard<std::mutex> lock(mNotifierMutex);
        mUiNotifier = {};
    }
}

void ClientApplication::processUserInputs(const int clientId) {
    std::string line;
    while (std::cout << "> " && std::getline(std::cin, line)) {
        if (line.empty()) {
            continue;
        }

        const Command command = parseCommand(line);
        switch (command.type) {
        case CommandType::BUY:
            submitOrder(clientId, TradeCore::OrderType::BUY, command.price, command.volume);
            break;
        case CommandType::SELL:
            submitOrder(clientId, TradeCore::OrderType::SELL, command.price, command.volume);
            break;
        case CommandType::EXIT:
            return;
        case CommandType::INVALID:
            pushStatus("Invalid command. Use: buy <price> <volume>, sell <price> <volume>, exit");
            break;
        }
    }
}

void ClientApplication::pushStatus(std::string message) {
    mStatusHistory.push(std::move(message));
    if (mOptions.mode != TradeCommon::ClientLaunchMode::Tui) {
        const auto items = statusSnapshot();
        if (!items.empty()) {
            std::cout << "[status] " << items.back() << '\n';
        }
    }
    notifyUi();
}

void ClientApplication::pushTrade(std::string message) {
    mTradeHistory.push(std::move(message));
    if (mOptions.mode != TradeCommon::ClientLaunchMode::Tui) {
        const auto items = tradeSnapshot();
        if (!items.empty()) {
            std::cout << "[trade] " << items.back() << '\n';
        }
    }
    notifyUi();
}

void ClientApplication::notifyUi() const {
    std::function<void()> notifier;
    {
        std::lock_guard<std::mutex> lock(mNotifierMutex);
        notifier = mUiNotifier;
    }
    if (notifier) {
        notifier();
    }
}

bool ClientApplication::sendEnvelope(const TradeCommon::Envelope& envelope) {
    std::lock_guard<std::mutex> lock(mClientMutex);
    if (!mClient || !mConnected.load()) {
        return false;
    }
    return mClient->sendMessage(TradeCommon::serializeEnvelope(envelope));
}

bool ClientApplication::submitOrder(const int clientId,
                                    const TradeCore::OrderType side,
                                    const int price,
                                    const int volume) {
    if (const auto error = TradeCommon::validateOrder(price, volume)) {
        pushStatus(*error);
        return false;
    }

    registerOrder(TradeCore::Order(clientId, side, price, volume));
    return true;
}

std::vector<std::string> ClientApplication::statusSnapshot() const {
    return mStatusHistory.snapshot();
}

std::vector<std::string> ClientApplication::tradeSnapshot() const {
    return mTradeHistory.snapshot();
}

ClientApplication::Command ClientApplication::parseCommand(const std::string& line) const {
    Command            command;
    std::istringstream iss(line);
    std::string        action;
    if (!(iss >> action)) {
        return command;
    }

    int price  = 0;
    int volume = 0;
    if (action == "buy" && iss >> price >> volume) {
        command.type   = CommandType::BUY;
        command.price  = price;
        command.volume = volume;
        return command;
    }
    if (action == "sell" && iss >> price >> volume) {
        command.type   = CommandType::SELL;
        command.price  = price;
        command.volume = volume;
        return command;
    }
    if (action == "exit") {
        command.type = CommandType::EXIT;
    }
    return command;
}

void ClientApplication::registerOrder(TradeCore::Order order) {
    const auto envelope = TradeCommon::makeSubmitOrderEnvelope(order.clientId, order.type, order.price, order.volume);
    if (!sendEnvelope(envelope)) {
        pushStatus("Failed to send order. Connect to the server first.");
        return;
    }

    std::ostringstream oss;
    oss << "Submitted " << TradeCommon::toString(order.type) << " order price=" << order.price
        << " volume=" << order.volume;
    pushStatus(oss.str());
}
