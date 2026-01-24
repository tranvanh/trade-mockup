#include "StockMarketGenerator.h"
#include "ClientApplication.h"
#include <atomic>
#include <chrono>
#include <functional>
#include <iomanip>
#include <thread>
#include <utility>
#include <UtilsLib/Logger.h>

constexpr int ID_COUNT = 100; // Possible Number of simulated IDs

int randomValueOfMax(const int max) {
    return std::rand() / ((RAND_MAX + 1u) / max);
}

StockMarketGenerator::StockMarketGenerator(ClientApplication& app)
    : mApplication(app) {
    std::srand(std::time(nullptr));
}

void StockMarketGenerator::simulateMarket() {
    while (mApplication.isRunning) {
        mApplication.runBackgroundTask(
            std::bind_front(&StockMarketGenerator::generateOrder, this, OrderType::BUY));
        mApplication.runBackgroundTask(
            std::bind_front(&StockMarketGenerator::generateOrder, this, OrderType::SELL));
    }
}

void StockMarketGenerator::generateOrder(OrderType type) {
    auto& logger = Logger::instance();
    logger.log(Logger::LogLevel::DEBUG, "Start generating orders ", type);
    logger.log(Logger::LogLevel::DEBUG, "Generating ", type);
    Order order(randomValueOfMax(ID_COUNT), type, randomValueOfMax(PRICE_MAX), randomValueOfMax(VOLUME_MAX));
    mApplication.registerOrder(std::move(order));
    std::this_thread::sleep_for(std::chrono::duration<int, std::milli>(randomValueOfMax(10) * 100));
}
