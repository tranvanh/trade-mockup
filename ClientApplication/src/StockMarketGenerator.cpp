#include "StockMarketGenerator.h"
#include "ClientApplication.h"
#include <chrono>
#include <functional>
#include <thread>
#include <Toybox/Logger.h>

constexpr int ID_COUNT = 100;

static int randomValueOfMax(const int max) {
    return std::rand() / ((RAND_MAX + 1u) / max);
}

StockMarketGenerator::StockMarketGenerator(ClientApplication& app)
    : mApplication(app) {
    std::srand(std::time(nullptr));
}

void StockMarketGenerator::start() {
    if (mSimActive.exchange(true)) {
        return;
    }
    mApplication.runBackgroundTask([this] { simulateMarket(); });
}

void StockMarketGenerator::stop() {
    mSimActive.store(false);
}

void StockMarketGenerator::simulateMarket() {
    auto& logger = toybox::Logger::instance();
    logger.log(toybox::Logger::LogLevel::DEBUG, "Simulation started");
    while (mSimActive && mApplication.isRunning) {
        for (int i = 0; i < 100; ++i) {
            mApplication.runBackgroundTask(
                std::bind_front(&StockMarketGenerator::generateOrder, this, TradeCore::OrderType::BUY));
            mApplication.runBackgroundTask(
                std::bind_front(&StockMarketGenerator::generateOrder, this, TradeCore::OrderType::SELL));
        }
        std::this_thread::sleep_for(std::chrono::seconds(2));
    }
    logger.log(toybox::Logger::LogLevel::DEBUG, "Simulation stopped");
}

void StockMarketGenerator::generateOrder(TradeCore::OrderType type) const{
    TradeCore::Order order(randomValueOfMax(ID_COUNT),
                           type,
                           randomValueOfMax(TradeCore::PRICE_MAX),
                           randomValueOfMax(TradeCore::VOLUME_MAX));
    mApplication.registerOrder(std::move(order));
}
