#include "StockMarketGenerator.h"
#include "Common.h"
#include "Application.h"
#include <chrono>
#include <iomanip>
#include <iostream>
#include <atomic>
#include <thread>
#include <utility>
#include <functional>

// APPLICATION_NAMESPACE_BEGIN

std::atomic<uint64_t> gIdCounter = 0;

int randomValueOfMax(const int max) {
    return std::rand() / ((RAND_MAX + 1u) / max);
}

StockMarketGenerator::StockMarketGenerator(TradeApp& app)
    : mApplication(app) {
    std::srand(std::time(nullptr));
}

void StockMarketGenerator::simulateMarket() {
    mStockMarket.run();
    mApplication.runBackgroundTask(std::bind_front(&StockMarketGenerator::generateOrder, this, OrderType::BUY));
    mApplication.runBackgroundTask(std::bind_front(&StockMarketGenerator::generateOrder, this, OrderType::SELL));
}

void StockMarketGenerator::generateOrder(OrderType type) {
    while(true){
        Order order;
        order.id = gIdCounter++;
        order.price = randomValueOfMax(10000);
        order.timeStamp = std::chrono::system_clock::now();
        order.type = type;
        order.volume = randomValueOfMax(1000);
        mStockMarket.registerOrder(order);
        std::this_thread::sleep_for(std::chrono::duration<int, std::milli>(randomValueOfMax(10)*100));
    }
}

// APPLICATION_NAMESPACE_END
