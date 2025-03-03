#include "Model.h"
#include "Common.h"
#include "TradeApp.h"
#include <chrono>
#include <iomanip>
#include <iostream>
#include <atomic>
#include <thread>
#include <utility>

TRADE_API_NAMESPACE_BEGIN

std::atomic<uint64_t> gIdCounter = 0;

int randomValueOfMax(const int max) {
    return std::rand() / ((RAND_MAX + 1u) / max);
}

std::ostream& operator<<(std::ostream& os, const timepoint_t& timepoint) {
    auto tp = std::chrono::system_clock::to_time_t(timepoint);
    return os << std::put_time(std::localtime(&tp), "%F %T");
}

std::ostream& operator<<(std::ostream& os, OrderType type) {
    switch (type) {
    case (OrderType::SELL):
        os << "SELL";
        break;
    case (OrderType::BUY):
        os << "BUY";
        break;
    default:
        CASSERT(true, "Trade type undeclared");
    }
    return os;
}

std::ostream& operator<<(std::ostream& os, const Order& order) {
    return os << "Order("
              << "id=" << order.id << ","
              << "timestamp=" << order.timeStamp << ","
              << "price=" << order.price << ","
              << "volume=" << order.volume << ","
              << "type=" << order.type << ")";
}

OrderModel::OrderModel(TradeApp& app)
    : mApplication(app) {
    std::srand(std::time(nullptr));
}

void OrderModel::simulateMarket() {
    mApplication.runBackgroundTask(std::bind_front(&OrderModel::generateOrder, this, OrderType::BUY));
    mApplication.runBackgroundTask(std::bind_front(&OrderModel::generateOrder, this, OrderType::SELL));
}

void OrderModel::generateOrder(OrderType type) {
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

TRADE_API_NAMESPACE_END
