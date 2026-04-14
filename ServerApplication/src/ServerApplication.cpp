#include "ServerApplication.h"
#include <TradeCommon/Protocol.h>
#include <ftxui/component/component.hpp>
#include <ftxui/component/component_base.hpp>
#include <ftxui/component/screen_interactive.hpp>
#include <ftxui/dom/elements.hpp>
#include <random>
#include <string_view>
#include <Toybox/Logger.h>
#include <TradeCore/Order.h>
#include <vector>

namespace {
    constexpr int            FILLED_COUNT = 100;
    constexpr unsigned short DEFAULT_PORT = 8080;

    std::string formatConnectionLine(const TradeCommon::TcpServer::ConnectionInfo& info,
                                     const std::string_view action) {
        return "Client #" + std::to_string(info.id) + ' ' + std::string(action) + " from " + info.address + ':'
            + std::to_string(info.port);
    }
}

void fillMarketWithMockData(TradeCore::Market& market, int count) {
    if (count <= 0)
        return;

    std::random_device rd;
    std::mt19937       gen(rd());

    std::uniform_int_distribution<int> typeDist(0, 1);
    std::uniform_int_distribution<int> priceDist(1, TradeCore::PRICE_MAX);
    std::uniform_int_distribution<int> volumeDist(1, TradeCore::VOLUME_MAX);
    std::uniform_int_distribution<int> clientDist(1, 1'000'000);

    for (int i = 0; i < count; ++i) {
        const auto type   = typeDist(gen) == 0 ? TradeCore::OrderType::BUY : TradeCore::OrderType::SELL;
        const int  price  = priceDist(gen);
        const int  volume = volumeDist(gen);
        const int  client = clientDist(gen);

        TradeCore::Order order(client, type, price, volume);
        market.registerOrder(order);
    }
}

ServerApplication::ServerApplication(TradeCommon::ServerLaunchOptions options)
    : Application(THREAD_COUNT)
    , mOptions(std::move(options))
    , mServer(DEFAULT_PORT)
    , mConnectionHistory(100)
    , mTradeHistory(100) {
    if (mOptions.filled) {
        pushConnectionEvent("Pre-filling the market with mock data...");
        fillMarketWithMockData(mStockMarket, FILLED_COUNT);
        pushConnectionEvent("Market filled with " + std::to_string(FILLED_COUNT) + " orders.");
    }

    registerCallback(mStockMarket.addOnTradeObserver([this](const TradeCore::Trade& trade) {
        pushTradeEvent(TradeCommon::formatTradeLine(trade));
        mServer.broadcast(TradeCommon::serializeEnvelope(TradeCommon::makeTradeEnvelope(trade)));
    }));

    mServer.onConnect = [this](const TradeCommon::TcpServer::ConnectionInfo& info) {
        pushConnectionEvent(formatConnectionLine(info, "connected"));
    };

    mServer.onDisconnect = [this](const TradeCommon::TcpServer::ConnectionInfo& info) {
        pushConnectionEvent(formatConnectionLine(info, "disconnected"));
    };

    mServer.onMessage = [this](const std::uint64_t connectionId, const std::string& msg) {
        runBackgroundTask([this, connectionId, msg]() { processServerMessage(connectionId, msg); });
    };
}

void ServerApplication::run() {
    Application::run();
    runBackgroundTask([this]() {
        mStockMarket.run();
    });

    if (mServer.start()) {
        pushConnectionEvent("Server listening on 0.0.0.0:" + std::to_string(DEFAULT_PORT) + '.');
    } else {
        pushConnectionEvent("Failed to start server on port " + std::to_string(DEFAULT_PORT) + '.');
    }

    runTui();
    mServer.stop();
    mStockMarket.stop();
    stop();
}

void ServerApplication::runTui() {
    using namespace ftxui;

    ScreenInteractive screen = ScreenInteractive::TerminalOutput();
    {
        std::lock_guard<std::mutex> lock(mNotifierMutex);
        mUiNotifier = [&screen]() { screen.PostEvent(Event::Custom); };
    }

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

    Component quitButton = Button("Quit", screen.ExitLoopClosure());
    Component root       = Renderer(quitButton, [&] {
        return vbox({
                   text("Trading Server Monitor") | bold,
                   text("Port: " + std::to_string(DEFAULT_PORT)),
                   text(std::string("Market prefill: ") + (mOptions.filled ? "enabled" : "disabled")),
                   separator(),
                   hbox({
                       renderHistoryWindow("Connections", connectionSnapshot()) | flex,
                       separator(),
                       renderHistoryWindow("Recent Trades", tradeSnapshot()) | flex,
                   }) | flex,
                   separator(),
                   quitButton->Render(),
               })
            | border;
    });

    screen.Loop(root);

    {
        std::lock_guard<std::mutex> lock(mNotifierMutex);
        mUiNotifier = {};
    }
}

void ServerApplication::processServerMessage(const std::uint64_t connectionId, const std::string& msg) {
    try {
        const auto envelope = TradeCommon::parseEnvelope(msg);
        if (envelope.type != TradeCommon::MessageType::SubmitOrder) {
            mServer.sendTo(connectionId,
                           TradeCommon::serializeEnvelope(
                               TradeCommon::makeErrorEnvelope("Only submit_order messages are accepted.")));
            return;
        }

        const auto order = TradeCommon::toOrder(std::get<TradeCommon::SubmitOrderPayload>(envelope.payload));
        mStockMarket.registerOrder(order);
    } catch (const std::exception& ex) {
        pushConnectionEvent("Rejected invalid client message: " + std::string(ex.what()));
        mServer.sendTo(connectionId,
                       TradeCommon::serializeEnvelope(
                           TradeCommon::makeErrorEnvelope(std::string("Invalid message: ") + ex.what())));
    }
}

void ServerApplication::pushConnectionEvent(std::string message) {
    mConnectionHistory.push(std::move(message));
    notifyUi();
}

void ServerApplication::pushTradeEvent(std::string message) {
    mTradeHistory.push(std::move(message));
    notifyUi();
}

void ServerApplication::notifyUi() const {
    std::function<void()> notifier;
    {
        std::lock_guard<std::mutex> lock(mNotifierMutex);
        notifier = mUiNotifier;
    }
    if (notifier) {
        notifier();
    }
}

std::vector<std::string> ServerApplication::connectionSnapshot() const {
    return mConnectionHistory.snapshot();
}

std::vector<std::string> ServerApplication::tradeSnapshot() const {
    return mTradeHistory.snapshot();
}
