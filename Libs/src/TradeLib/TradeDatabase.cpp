#include "TradeLib/TradeDatabase.h"
// #include "Application.h"
#include <iostream>

// TRADE_NAMESPACE_BEGIN

void TradeDatabase::registerTrade(const Trade& trade) {
    std::lock_guard<std::mutex> tradeLock(mTrades.lock);
    mTrades.data.insert(trade);
}

// TRADE_NAMESPACE_END