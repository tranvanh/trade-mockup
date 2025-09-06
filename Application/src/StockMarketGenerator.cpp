#include "StockMarketGenerator.h"
#include "TradeApp.h"
#include "UtilsLib/Logger.h"
#include <chrono>
#include <iomanip>
#include <iostream>
#include <atomic>
#include <thread>
#include <utility>
#include <utility>
#include <functional>

// APPLICATION_NAMESPACE_BEGIN

std::atomic<uint64_t> gIdCounter = 0;

int randomValueOfMax(const int max) {
    return std::rand() / ((RAND_MAX + 1u) / max);
}

StockMarketGenerator::StockMarketGenerator(TradeApp& app)
    : mApplication(app), mStockMarket(app.getStockMarket()) {
    std::srand(std::time(nullptr));
}

void StockMarketGenerator::simulateMarket() {
    mStockMarket.run();
    mApplication.runBackgroundTask(std::bind_front(&StockMarketGenerator::generateOrder, this, OrderType::BUY));
    mApplication.runBackgroundTask(std::bind_front(&StockMarketGenerator::generateOrder, this, OrderType::SELL));
}

void StockMarketGenerator::generateOrder(OrderType type) {
    auto& logger = Logger::instance();
    logger.log(Logger::LogLevel::DEBUG, "Start generating orders ", type);

    while(mApplication.isRunning){
        Order order;
        order.id = gIdCounter++;
        order.price = randomValueOfMax(PRICE_MAX);
        order.timeStamp = std::chrono::system_clock::now();
        order.type = type;
        order.volume = randomValueOfMax(VOLUME_MAX);
        mStockMarket.registerOrder(order);
        std::this_thread::sleep_for(std::chrono::duration<int, std::milli>(randomValueOfMax(10)*100));
    }
}

// APPLICATION_NAMESPACE_END
