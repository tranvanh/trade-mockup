#include "ServerApplication.h"
#include <nlohmann/json.hpp>
#include <chrono>
#include <sstream>
#include <string>
#include <thread>
#include <Toybox/Logger.h>
#include <TradeCore/Order.h>

ServerApplication::ServerApplication()
    : Application(SERVER_THREAD_COUNT/2)
    , mServer(8080, SERVER_THREAD_COUNT/2)
    , mUIStream([this](std::string line) {
        mUIState.pushLog(std::move(line));
    })
    , mScreen(ftxui::ScreenInteractive::Fullscreen()) {
    registerCallback(mStockMarket.addOnTradeObserver(
        [this](const TradeCore::Trade& t) { onTradeExecuted(t); }));
}

void ServerApplication::run() {
    Application::run();
    toybox::Logger::instance().setOutputStream(mUIStream);
    mServer.onConnect = [this](unsigned short port) {
        mUIState.addConnection(port);
        mUIState.pushLog("[INFO] Client connected on port " + std::to_string(port));
    };

    mServer.onDisconnect = [this](unsigned short port) {
        mUIState.removeConnection(port);
        mUIState.pushLog("[INFO] Client disconnected from port " + std::to_string(port));
    };

    mServer.onRecieve = [this](std::string msg) {
        // \todo instead of running background task for every received message. Add it to a msg queue and drain from separate queue
        runBackgroundTask([this, msg] { processServerMessage(msg); });
    };

    runBackgroundTask([this] { mStockMarket.run(); });
    runBackgroundTask([this] {
        while (isRunning) {
            std::this_thread::sleep_for(std::chrono::milliseconds(33));
            mScreen.PostEvent(ftxui::Event::Custom);
        }
    });
    mServer.run();
}

void ServerApplication::stop() {
    mServer.onConnect    = nullptr;
    mServer.onDisconnect = nullptr;
    mServer.onRecieve    = nullptr;
    mStockMarket.stop();
    Application::stop();
}

void ServerApplication::onTradeExecuted(const TradeCore::Trade& trade) {
    std::ostringstream oss;
    oss << "TRADE B#" << trade.buyerId
        << " S#" << trade.sellerId
        << " @" << trade.price
        << " x" << trade.volume;
    mUIState.pushTrade(oss.str());

    nlohmann::json j;
    j["sellerId"] = trade.sellerId;
    j["buyerId"]  = trade.buyerId;
    j["price"]    = trade.price;
    j["volume"]   = trade.volume;
    mServer.broadcast(nlohmann::to_string(j));
}

void ServerApplication::processServerMessage(const std::string& msg) {
    try {
        auto j = nlohmann::json::parse(msg);
        int  clientId = j["clientId"].get<int>();
        int  price    = j["price"].get<int>();
        int  volume   = j["volume"].get<int>();
        auto type     = j["type"].get<int>() == 0
                            ? TradeCore::OrderType::BUY
                            : TradeCore::OrderType::SELL;

        std::ostringstream oss;
        oss << "[RECV] client=" << clientId
            << " " << (type == TradeCore::OrderType::BUY ? "BUY" : "SELL")
            << " @" << price << " x" << volume;
        mUIState.pushLog(oss.str());

        TradeCore::Order order(clientId, type, price, volume);
        mStockMarket.registerOrder(order);
    } catch (const std::exception& e) {
        mUIState.pushLog(std::string("[ERROR] Bad message: ") + e.what());
    }
}
