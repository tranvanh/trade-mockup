#pragma once
#include "TradeLib/Order.h"
#include "UtilsLib/ThreadSafeQueue.h"
#include <condition_variable>
#include <deque>
#include <forward_list>
#include <map>
#include <thread>

TRANVANH_NAMESPACE_BEGIN

class StockMarket;

// Match orders the highest buyer with lowest seller
class OrderBook {
    std::forward_list<std::thread> mThreadPool;

    StockMarket&           mStockMarket;
    ThreadSafeQueue<Order> mBuyerQueue;
    ThreadSafeQueue<Order> mSellerQueue;

    struct {
        std::mutex                                   lock;
        std::multimap<int, Order, std::greater<int>> data;
    } mBuyers;

    struct {
        std::mutex                lock;
        std::multimap<int, Order> data;
    } mSellers;

public:
    OrderBook(StockMarket& stockMarket);
    ~OrderBook();
    void registerOrder(const Order& order);
    void run();

private:
    void processBuyers();
    void processSellers();

    // We allow orders with negative volumes due to performance. After a certain time we clean invalid orders
    void cleanUpBuyers();
    void cleanUpSellers();

    int getSoldVolumes(const int buyer, const int seller) const;

    // Make a trade and update volumes
    void matchOrders(Order& buyer, Order& seller);
};

TRANVANH_NAMESPACE_END
