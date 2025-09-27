#pragma once
#include "TradeLib/Order.h"
#include "UtilsLib/ThreadSafeQueue.h"
#include <condition_variable>
#include <deque>
#include <map>
#include <thread>
#include <unordered_set>


TRANVANH_NAMESPACE_BEGIN

class StockMarket;

/// We receive orders through one serialized queue. Order then gets processed, the order book is always in
/// valid state. All is done on main thread, but note that registerOrder can be called from different threads
class OrderBook {
    StockMarket&                                 mStockMarket;
    ThreadSafeQueue<Order>                       mOrderQueue;
    std::multimap<int, Order, std::greater<int>> mBuyers;
    std::multimap<int, Order>                    mSellers;

public:
    OrderBook(StockMarket& stockMarket);
    ~OrderBook();
    void registerOrder(const Order& order);
    void run();

private:
    void processBuyer(Order buyer);
    void processSeller(Order seller);

    void cleanUpBuyers(const std::unordered_set<int>& toRemove);
    void cleanUpSellers(const std::unordered_set<int>& toRemove);

    int getSoldVolumes(const int buyer, const int seller) const;

    // Make a trade and update volumes
    void matchOrders(Order& buyer, Order& seller);
};

TRANVANH_NAMESPACE_END
