#include "Model.h"
#include "Common.h"
#include <chrono>
#include <format>
#include <iomanip>
#include <iostream>
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

std::ostream& operator<<(std::ostream& os, TradeType tradeType) {
    switch (tradeType) {
    case (TradeType::SELL):
        os << "SELL";
        break;
    case (TradeType::BUY):
        os << "BUY";
        break;
    default:
        CASSERT(true, "Trade type undeclared");
    }
    return os;
}


std::ostream& operator<<(std::ostream& os, const Trade& trade) {
    return os << "Trade("
              << "trade_id=" << trade.id << ","
              << "timestamp=" << trade.timeStamp << ","
              << "price=" << trade.price << ","
              << "volume=" << trade.volume << ","
              << "type=" << trade.tradeType << ")";
}

TradeModel::TradeModel() {
    std::srand(std::time(nullptr));
}

void TradeModel::generateTrade(TradeType tradeType) {
    Trade trade;
    trade.id = gIdCounter++;
    trade.price = randomValueOfMax(10000);
    trade.timeStamp = std::chrono::system_clock::now();
    trade.tradeType = tradeType;
    trade.volume = randomValueOfMax(1000);
    std::cout << trade << std::endl;
    std::this_thread::sleep_for(std::chrono::duration<int, std::milli>(randomValueOfMax(10)*100));
}

void TradeModel::simulateMarket() {
    int running = 15;
    while (running--) {
        generateTrade(TradeType::BUY);
        generateTrade(TradeType::SELL);
    }
}

TRADE_API_NAMESPACE_END
