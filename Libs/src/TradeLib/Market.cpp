#include "TradeLib/Market.h"
#include "UtilsLib/Logger.h"

TRANVANH_NAMESPACE_BEGIN

void Market::run() {
    auto& logger = Logger::instance();
    logger.log(Logger::LogLevel::DEBUG, "Initialize Stock market");
    mActive = true;
    logger.log(Logger::LogLevel::DEBUG, "Initialize Order book");
    while (mActive) {
        mBook.processOrders();
    }
}

void Market::registerOrder(const Order& order) {
    // register queue

    // get the book data
    // if null_opt -> insert to book
    //  else
    // start processing the key and make order -> registerTrades
    // if remaining volume receive, repeat

    mBook.registerOrder(order);

}

void Market::registerTrade(const Trade& trade) {
    mOnTradeCallbacks(trade);
}

[[nodiscard]] CallbackLifetime Market::addOnTradeObserver(const std::function<void(const Trade& trade)>& callback){
    return mBook.onTradeCallbacks.add(callback);
}

TRANVANH_NAMESPACE_END