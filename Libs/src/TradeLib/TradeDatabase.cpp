#include "TradeLib/TradeDatabase.h"

TRANVANH_NAMESPACE_BEGIN

void TradeDatabase::registerTrade(const Trade& trade) {
    std::lock_guard<std::mutex> tradeLock(mTrades.lock);
    mTrades.data.insert(trade);
}

TRANVANH_NAMESPACE_END