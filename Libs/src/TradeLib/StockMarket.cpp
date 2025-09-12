#include "TradeLib/StockMarket.h"
#include "UtilsLib/Logger.h"

TRANVANH_NAMESPACE_BEGIN

void StockMarket::run() {
    auto& logger = Logger::instance();
    logger.log(Logger::LogLevel::DEBUG, "Initialize Stock market");
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

TRANVANH_NAMESPACE_END