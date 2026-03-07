#pragma once
#include "Toybox/CallbackList.h"
#include "Toybox/FlatMap.h"
#include "Toybox/ThreadSafeQueue.h"
#include "TradeCore/Order.h"
#include "TradeCore/Trade.h"


TRANVANH_NAMESPACE_BEGIN

class Market;

/// We receive orders through one serialized queue. Order then gets processed, the order book is always in
/// valid state. All is done on main thread, but note that registerOrder can be called from different threads
class OrderBook {
    struct PriceLevel {
        int               price  = 0;
        int               volume = 0;
        std::deque<Order> orders;
    };

    ThreadSafeQueue<Order> mOrderQueue;

    // \todo use one single container and reduce code
    // \todo try to use flat container
    FlatMap<int, std::shared_ptr<PriceLevel>> mBuyers;
    FlatMap<int, std::shared_ptr<PriceLevel>> mSellers;

public:
    OrderBook();
    ~OrderBook();
    void registerOrder(const Order& order);
    void pollOrders();

    CallbackList<void(const Trade& trade)> onTradeCallbacks;

private:
    void processOrder(const Order& buyer);
    void insertBuyer(const Order& buyer);
    void insertSeller(const Order& seller);
    void matchOrders(Order& buyer, PriceLevel& seller);

    void processBuyer(Order buyer);
    void processSeller(Order seller);

    int getSoldVolumes(const int buyer, const int seller) const;
};

TRANVANH_NAMESPACE_END
