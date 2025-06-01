#include "MarketGenerator.h"
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

MarketGenerator::MarketGenerator(TradeApp& app)
    : mApplication(app) {
    std::srand(std::time(nullptr));
}

void MarketGenerator::simulateMarket() {
    mApplication.runBackgroundTask(std::bind_front(&MarketGenerator::generateOrder, this, OrderType::BUY));
    mApplication.runBackgroundTask(std::bind_front(&MarketGenerator::generateOrder, this, OrderType::SELL));
}

void MarketGenerator::generateOrder(OrderType type) {
    while(true){
        Order order;
        order.id = gIdCounter++;
        order.price = randomValueOfMax(10000);
        order.timeStamp = std::chrono::system_clock::now();
        order.type = type;
        order.volume = randomValueOfMax(1000);
        mApplication.registerOrder(order);
        std::this_thread::sleep_for(std::chrono::duration<int, std::milli>(randomValueOfMax(10)*100));
    }
}

// APPLICATION_NAMESPACE_END
