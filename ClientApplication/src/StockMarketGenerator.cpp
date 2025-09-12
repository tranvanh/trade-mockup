#include "StockMarketGenerator.h"
#include "ClientApplication.h"
#include <UtilsLib/Logger.h>
#include <chrono>
#include <iomanip>
#include <iostream>
#include <atomic>
#include <thread>
#include <utility>
#include <utility>
#include <functional>

// APPLICATION_NAMESPACE_BEGIN

constexpr int ID_COUNT  = 100; // Possible Number of simulated IDs

int randomValueOfMax(const int max) {
    return std::rand() / ((RAND_MAX + 1u) / max);
}

StockMarketGenerator::StockMarketGenerator(ClientApplication& app)
    : mApplication(app) {
    std::srand(std::time(nullptr));
}

void StockMarketGenerator::simulateMarket() {
    mApplication.runBackgroundTask(std::bind_front(&StockMarketGenerator::generateOrder, this, OrderType::BUY));
    mApplication.runBackgroundTask(std::bind_front(&StockMarketGenerator::generateOrder, this, OrderType::SELL));
}

void StockMarketGenerator::generateOrder(OrderType type) {
    auto& logger = Logger::instance();
    logger.setLevel(Logger::LogLevel::DEBUG);
    logger.log(Logger::LogLevel::DEBUG, "Start generating orders ", type);

    while(mApplication.isRunning){
        logger.log(Logger::LogLevel::DEBUG, "Generating ", type);
        Order order;
        order.id = randomValueOfMax(ID_COUNT);
        order.price = randomValueOfMax(PRICE_MAX);
        order.type = type;
        order.volume = randomValueOfMax(VOLUME_MAX);
        mApplication.registerOrder(std::move(order));
        std::this_thread::sleep_for(std::chrono::duration<int, std::milli>(randomValueOfMax(10)*100));
    }
}

// APPLICATION_NAMESPACE_END
