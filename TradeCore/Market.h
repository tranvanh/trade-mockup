#pragma once
#include "Toybox/CallbackList.h"
#include "TradeCore/OrderBook.h"
#include "TradeCore/Trade.h"
#include <atomic>

namespace TradeCore {

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

        [[nodiscard]] toybox::CallbackLifetime addOnTradeObserver(
            const std::function<void(const Trade& trade)>& callback);
    };
}
