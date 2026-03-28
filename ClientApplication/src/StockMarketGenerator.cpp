#include "StockMarketGenerator.h"
#include "ClientApplication.h"
#include <atomic>
#include <chrono>
#include <functional>
#include <iomanip>
#include <thread>
#include <Toybox/Logger.h>
#include <utility>

constexpr int ID_COUNT = 100; // Possible Number of simulated IDs

int randomValueOfMax(const int max) {
    return std::rand() / ((RAND_MAX + 1u) / max);
}

StockMarketGenerator::StockMarketGenerator(ClientApplication& app)
    : mApplication(app) {
    std::srand(std::time(nullptr));
}

void StockMarketGenerator::simulateMarket() {
    auto& logger = toybox::Logger::instance();
    logger.log(toybox::Logger::LogLevel::DEBUG, "Start generating orders ");
    while (mApplication.isRunning) {
        for (int i = 0; i < 100; ++i) {
            mApplication.runBackgroundTask(
                std::bind_front(&StockMarketGenerator::generateOrder, this, TradeCore::OrderType::BUY));
            mApplication.runBackgroundTask(
                std::bind_front(&StockMarketGenerator::generateOrder, this, TradeCore::OrderType::SELL));
        }
        // Sleep in short intervals so shutdown is noticed quickly
        for (int i = 0; i < 20 && mApplication.isRunning; ++i)
            std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }
}

void StockMarketGenerator::generateOrder(TradeCore::OrderType type) {
    if (!mApplication.isRunning) return;
    auto& logger = toybox::Logger::instance();
    logger.log(toybox::Logger::LogLevel::DEBUG, "Generating ", type);
    TradeCore::Order order(randomValueOfMax(ID_COUNT),
                           type,
                           randomValueOfMax(TradeCore::PRICE_MAX),
                           randomValueOfMax(TradeCore::VOLUME_MAX));
    mApplication.registerOrder(std::move(order));
    if (!mApplication.isRunning) return;
    std::this_thread::sleep_for(std::chrono::duration<int, std::milli>(randomValueOfMax(10) * 100));
}
