#include "ServerApplication.h"
#include <nlohmann/json.hpp>
#include <sstream>
#include <string>
#include <Toybox/Logger.h>
#include <TradeCore/Order.h>

ServerApplication::ServerApplication()
    : Application(SERVER_THREAD_COUNT/2)
    , mServer(8080, SERVER_THREAD_COUNT/2)
    , mUIStream([this](std::string line) {
        mUIState.pushLog(std::move(line));
        mScreen.PostEvent(ftxui::Event::Custom);
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
        mScreen.PostEvent(ftxui::Event::Custom);
    };

    mServer.onDisconnect = [this](unsigned short port) {
        mUIState.removeConnection(port);
        mUIState.pushLog("[INFO] Client disconnected from port " + std::to_string(port));
        mScreen.PostEvent(ftxui::Event::Custom);
    };

    mServer.onRecieve = [this](std::string msg) {
        runBackgroundTask([this, msg] { processServerMessage(msg); });
    };

    runBackgroundTask([this] { mStockMarket.run(); });
    mServer.run();
}

void ServerApplication::stop() {
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
    mScreen.PostEvent(ftxui::Event::Custom);

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
        mScreen.PostEvent(ftxui::Event::Custom);

        TradeCore::Order order(clientId, type, price, volume);
        mStockMarket.registerOrder(order);
    } catch (const std::exception& e) {
        mUIState.pushLog(std::string("[ERROR] Bad message: ") + e.what());
        mScreen.PostEvent(ftxui::Event::Custom);
    }
}
