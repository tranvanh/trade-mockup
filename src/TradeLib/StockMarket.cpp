#include "StockMarket.h"

void StockMarket::run() {
    mActive = true;
    mBook.run();
}

void StockMarket::registerOrder(const Order& order) {
    mBook.registerOrder(order);
}

void StockMarket::registerTrade(const Trade& trade) {
    mDatabase.registerTrade(trade);
    mOnTradeCallbacks(trade);
}

[[nodiscard]] CallbackLifetime StockMarket::addOnTradeObserver(const std::function<void(const Trade& trade)>& callback){
    return mOnTradeCallbacks.add(callback);
}