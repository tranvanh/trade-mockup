#include "TradeDatabase.h"
#include "Application.h"
#include <iostream>

// TRADE_NAMESPACE_BEGIN

TradeDatabase::TradeDatabase(TradeApp& application)
    : mApplication(application) {}

void TradeDatabase::registerTrade(const Trade& trade){
    std::lock_guard<std::mutex> tradeLock(mTrades.lock);
    std::cout << trade << std::endl; // \todo logger should run on other thread
    mTrades.data.insert(trade);
}

// TRADE_NAMESPACE_END