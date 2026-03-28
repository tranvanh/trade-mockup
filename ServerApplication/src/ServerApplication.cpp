#include "ServerApplication.h"
#include "ServerCli.h"
#include <nlohmann/json.hpp>
#include <random>
#include <stdexcept>
#include <string>
#include <Toybox/Logger.h>
#include <TradeCore/Order.h>

constexpr int THREAD_COUNT = 6;
constexpr int FILLED_COUNT = 100;

static void fillMarketWithMockData(TradeCore::Market& market, int count) {
    if (count <= 0)
        return;
    std::mt19937 gen{std::random_device{}()};
    std::uniform_int_distribution<int> typeDist(0, 1);
    std::uniform_int_distribution<int> priceDist(1, TradeCore::PRICE_MAX);
    std::uniform_int_distribution<int> volumeDist(1, TradeCore::VOLUME_MAX);
    std::uniform_int_distribution<int> clientDist(1, 1'000'000);
    for (int i = 0; i < count; ++i) {
        TradeCore::Order order(
            clientDist(gen),
            typeDist(gen) == 0 ? TradeCore::OrderType::BUY : TradeCore::OrderType::SELL,
            priceDist(gen),
            volumeDist(gen));
        market.registerOrder(order);
    }
}

ServerApplication::ServerApplication()
    : Application(THREAD_COUNT) {}

void ServerApplication::run() {
    ServerCli cli(*this);

    // Wire CLI callbacks before starting background threads so no events are missed
    setOnEvent([&cli](std::string msg) { cli.appendEvent(std::move(msg)); });
    setOnTrade([&cli](const TradeCore::Trade& t) { cli.appendTrade(t); });

    cli.run();  // blocks until user quits; calls stop() before returning

    // All threads joined at this point. Clear lambdas that captured cli by reference
    // before cli goes out of scope.
    setOnEvent(nullptr);
    setOnTrade(nullptr);
}  // cli destroyed here; ServerApplication outlives it

void ServerApplication::startBackground(int port, bool filled) {
    try {
        mServer.emplace(static_cast<short>(port));

        if (filled) {
            emitEvent("Filling market with " + std::to_string(FILLED_COUNT) + " mock orders...");
            fillMarketWithMockData(mStockMarket, FILLED_COUNT);
            emitEvent("Market filled with " + std::to_string(FILLED_COUNT) + " orders");
        }

        registerCallback(mStockMarket.addOnTradeObserver([this](const TradeCore::Trade& trade) {
            if (mOnTrade) mOnTrade(trade);
        }));

        Application::run();

        runBackgroundTask([this]() { mStockMarket.run(); });

        mServer->onConnect = [this](int socketId) {
            emitEvent("Client connected    (socket " + std::to_string(socketId) + ")");
        };
        mServer->onDisconnect = [this](int socketId) {
            emitEvent("Client disconnected (socket " + std::to_string(socketId) + ")");
        };
        mServer->onRecieve = [this](std::string msg) {
            runBackgroundTask([this, msg = std::move(msg)]() {
                processServerMessage(msg);
            });
        };

        runBackgroundTask([this]() { mServer->run(); });

    } catch (const std::exception& e) {
        throw std::runtime_error(std::string("Server failed to start: ") + e.what());
    }
}

void ServerApplication::stop() {
    mStockMarket.stop();  // sets mActive=false, stops order queue → market thread exits
    mServer.reset();      // ~Server() → mContext.stop() → server thread exits
    Application::stop();  // joins all threads (now clean)
}

void ServerApplication::emitEvent(std::string msg) {
    if (mOnEvent) mOnEvent(std::move(msg));
}

void ServerApplication::processServerMessage(const std::string& msg) {
    try {
        nlohmann::json json = nlohmann::json::parse(msg);
        int clientId = json["clientId"].get<int>();
        int price    = json["price"].get<int>();
        int volume   = json["volume"].get<int>();
        TradeCore::OrderType type =
            json["type"].get<int>() == 0 ? TradeCore::OrderType::BUY : TradeCore::OrderType::SELL;
        TradeCore::Order order(clientId, type, price, volume);
        mStockMarket.registerOrder(order);
    } catch (const std::exception& e) {
        toybox::Logger::instance().log(toybox::Logger::LogLevel::ERROR,
            "Failed to process message: ", e.what());
    }
}
