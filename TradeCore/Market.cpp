#include "TradeCore/Market.h"
#include "Toybox/Logger.h"

namespace TradeCore {
    void Market::run() {
        auto& logger = toybox::Logger::instance();
        logger.log(toybox::Logger::LogLevel::DEBUG, "Initialize Stock market");
        mActive = true;
        logger.log(toybox::Logger::LogLevel::DEBUG, "Initialize Order book");
        while (mActive) {
            mBook.pollOrders();
        }

    }

    void Market::registerOrder(const Order& order) {
        mBook.registerOrder(order);
    }

    [[nodiscard]] toybox::CallbackLifetime Market::addOnTradeObserver(const std::function<void(const Trade& trade)>& callback){
        return mBook.onTradeCallbacks.add(callback);
    }
}