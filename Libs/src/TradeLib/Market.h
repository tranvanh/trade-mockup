#pragma once
#include "TradeLib/OrderBook.h"
#include <atomic>
#include "UtilsLib/CallbackList.h"
#include "TradeLib/Trade.h"

TRANVANH_NAMESPACE_BEGIN
class Market {
    // \todo add more books for various symbols. Each running in its own thread
    // \todo add publisher
    OrderBook        mBook;
    std::atomic_bool mActive = false;

public:
    ~Market() { mActive = false; };

    void run();
    bool isActive() { return mActive; }
    void registerOrder(const Order& order);

    [[nodiscard]] CallbackLifetime addOnTradeObserver(
        const std::function<void(const Trade& trade)>& callback);

};

TRANVANH_NAMESPACE_END
