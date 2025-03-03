#pragma once

#include "Model.h"
#include <condition_variable>
#include <deque>
#include <functional>
#include <map>
#include <queue>
#include <set>
#include <thread>
#include <unordered_map>

TRADE_API_NAMESPACE_BEGIN

class TradeApp;

template <typename Type>
class ThreadSafeQueue {
    std::mutex              m;
    std::condition_variable cv;
    std::deque<Type>        mQueue;

public:
    void push(const Type& value);
    Type pop();
    bool empty() { return mQueue.empty(); }
};

// we need order book [x]
// store trades which happened

// must be thread safe queue
// producer consumer
// thread safe database
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
    // store all trades
    // search by id
    // search by timestamp
private:
    void processBuyers();
    void processSellers();

    void cleanUpBuyers();
    void cleanUpSellers();
};

TRADE_API_NAMESPACE_END
