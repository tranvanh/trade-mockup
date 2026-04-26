#include "ClientApplication.h"
#include <nlohmann/json.hpp>
#include <chrono>
#include <random>
#include <sstream>
#include <thread>
#include <Toybox/Logger.h>

uint ClientApplication::generateId() {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<uint> dist(1, 999999);
    return dist(gen);
}

ClientApplication::ClientApplication()
    : toybox::Application(CLIENT_THREAD_COUNT)
    , mId(generateId())
    , mUIStream([this](std::string line) {
        mUIState.pushLog(std::move(line));
    })
    , mGenerator(*this)
    , mScreen(ftxui::ScreenInteractive::Fullscreen()) {}

void ClientApplication::run() {
    Application::run();
    toybox::Logger::instance().setOutputStream(mUIStream);

    mClient.onReceive = [this](std::string msg) {
        try {
            auto j = nlohmann::json::parse(msg);
            std::ostringstream oss;
            oss << "TRADE B#" << j["buyerId"].get<int>()
                << " S#" << j["sellerId"].get<int>()
                << " @" << j["price"].get<int>()
                << " x" << j["volume"].get<int>();
            mUIState.pushTrade(oss.str());
        } catch (...) {
            mUIState.pushLog("Received unparseable message");
        }
    };

    if (!mClient.connect("127.0.0.1", 8080)) {
        mUIState.pushLog("[ERROR] Failed to connect to server at 127.0.0.1:8080");
    } else {
        mUIState.pushLog("[INFO] Connected to server");
    }

    runBackgroundTask([this] { mClient.run(); });
    runBackgroundTask([this] {
        while (isRunning) {
            std::this_thread::sleep_for(std::chrono::milliseconds(33));
            mScreen.PostEvent(ftxui::Event::Custom);
        }
    });
}

void ClientApplication::stop() {
    mGenerator.stop();
    mClient.stop();
    Application::stop();
}

void ClientApplication::registerOrder(TradeCore::Order order) const {
    nlohmann::json j;
    j["clientId"] = order.clientId;
    j["type"]     = static_cast<int>(order.type);
    j["price"]    = order.price;
    j["volume"]   = order.volume;
    mClient.sendMessage(nlohmann::to_string(j));
}

void ClientApplication::subscribe() {
    mClient.subscribe();
    mUIState.pushLog("[INFO] Subscribed to trade feed");
}

void ClientApplication::toggleSimulate() {
    if (mGenerator.isActive()) {
        mGenerator.stop();
        mUIState.pushLog("[INFO] Simulation stopped");
    } else {
        mGenerator.start();
        mUIState.pushLog("[INFO] Simulation started");
    }
}

bool ClientApplication::isSimulating() const {
    return mGenerator.isActive();
}
