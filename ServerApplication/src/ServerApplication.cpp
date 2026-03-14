#include "ServerApplication.h"
#include <functional>
#include <nlohmann/json.hpp>
#include <random>
#include <string>
#include <Toybox/Logger.h>
#include <TradeCore/Order.h>
#include <vector>

constexpr int THREAD_COUNT = 6;
constexpr int FILLED_COUNT = 100;

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

ServerApplication::ServerApplication(const bool filled)
    : Application(THREAD_COUNT)
    , mServer(8080) {
    auto& logger = toybox::Logger::instance();
    if (filled) {
        logger.log(toybox::Logger::LogLevel::INFO, "Filling the market...");
        fillMarketWithMockData(mStockMarket, FILLED_COUNT);
        logger.log(toybox::Logger::LogLevel::INFO, "Market filled with ", FILLED_COUNT, " orders.");
    }

    registerCallback(mStockMarket.addOnTradeObserver([&logger](const TradeCore::Trade& trade) {
        logger.log(toybox::Logger::LogLevel::INFO, trade);
    }));
}

void ServerApplication::run() {
    toybox::Application::run();
    auto& logger = toybox::Logger::instance();
    logger.log(toybox::Logger::LogLevel::DEBUG, "Initialize application");
    runBackgroundTask([this]() {
        mStockMarket.run();
    });

    mServer.onRecieve = [this](std::string msg) {
        auto& logger = toybox::Logger::instance();
        logger.log(toybox::Logger::LogLevel::INFO, "Received message...");
        runBackgroundTask([this, msg]() {
            processServerMessage(msg);
        });
    };
    mServer.run();
}

void ServerApplication::processServerMessage(const std::string& msg) {
    auto& logger = toybox::Logger::instance();
    logger.log(toybox::Logger::LogLevel::INFO, "Processing server message...", msg);

    int                  clientId = 0;
    int                  price    = 0;
    int                  volume   = 0;
    TradeCore::OrderType type     = TradeCore::OrderType::BUY;
    nlohmann::json       msgJson  = nlohmann::json::parse(msg);
    msgJson["clientId"].get_to(clientId);
    msgJson["price"].get_to(price);
    msgJson["volume"].get_to(volume);
    type = msgJson["type"].get<int>() == 0 ? TradeCore::OrderType::BUY : TradeCore::OrderType::SELL;
    TradeCore::Order order(clientId, type, price, volume);
    mStockMarket.registerOrder(order);
}
