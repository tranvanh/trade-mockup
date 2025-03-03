#include "TradeDatabase.h"
#include "TradeApp.h"
#include "Model.h"
#include <iostream>

TRADE_API_NAMESPACE_BEGIN


std::ostream& operator<<(std::ostream& os, const Trade& trade) {
    return os << "Trade" << "\n"
             << "Seller" << trade.seller << "\n"
             << "Buyer" << trade.buyer << "\n"
              << "time =" << trade.tradeTime << ","
              << "volume=" << trade.volume;
}

TradeDatabase::TradeDatabase(TradeApp& application)
    : mApplication(application) {}

void TradeDatabase::registerTrade(const Trade& trade){
    std::lock_guard<std::mutex> tradeLock(mTrades.lock);
    std::cout << trade << std::endl;
    mTrades.data.insert(trade);
}
TRADE_API_NAMESPACE_END