#include "TradeLib/Market.h"
#include "UtilsLib/Logger.h"

TRANVANH_NAMESPACE_BEGIN

void Market::run() {
    auto& logger = Logger::instance();
    logger.log(Logger::LogLevel::DEBUG, "Initialize Stock market");
    mActive = true;
    logger.log(Logger::LogLevel::DEBUG, "Initialize Order book");
    while (mActive) {
        mBook.pollOrders();
    }

}

void Market::registerOrder(const Order& order) {
    mBook.registerOrder(order);
}

[[nodiscard]] CallbackLifetime Market::addOnTradeObserver(const std::function<void(const Trade& trade)>& callback){
    return mBook.onTradeCallbacks.add(callback);
}

TRANVANH_NAMESPACE_END