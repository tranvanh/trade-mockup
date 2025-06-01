#pragma once

#include "Model.h"
#include <condition_variable>
#include <deque>
#include <map>
#include <thread>
#include "UtilsLib/ThreadSafeQueue.h"

TRADE_API_NAMESPACE_BEGIN

class TradeApp;

class OrderBook {
    TradeApp&              mApplication;
    ThreadSafeQueue<Order> mBuyerQueue;
    ThreadSafeQueue<Order> mSellerQueue;

    struct {
        std::mutex                            lock;
        std::multimap<int, Order, std::greater<int>> data;
    } mBuyers;

    struct {
        std::mutex         lock;
        std::multimap<int, Order> data;
    } mSellers;

public:
    OrderBook(TradeApp& app);
    void registerOrder(const Order& t);
    void run();

private:
    void processBuyers();
    void processSellers();

    void cleanUpBuyers();
    void cleanUpSellers();

    int getSoldVolumes(const int buyer, const int seller) const;
    void matchOrders(Order& buyer, Order& seller);
};

TRADE_API_NAMESPACE_END
