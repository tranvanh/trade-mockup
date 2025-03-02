#pragma once

#include "Model.h"
#include <condition_variable>
#include <deque>
#include <functional>
#include <queue>
#include <thread>
#include <unordered_map>
#include <map>
#include <set>
#include <vector>

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
class TradeBook {
    TradeApp&                           mApplication;
    ThreadSafeQueue<Trade>              mQueue;

    struct{
        std::mutex lock;
        std::multimap<int, Trade> data;
    } mBuyers;

    struct{
        std::mutex lock;
        std::multimap<int, Trade> data;
    } mSellers;

public:
    TradeBook(TradeApp& app);
    void registerTrade(const Trade& t);
    void run();
    // store all trades
    // search by id
    // search by timestamp
private:
void processTrades();
void processBuyer(Trade trade);
void processSeller(Trade trade);

void cleanUpBuyers();
void cleanUpSellers();
};

TRADE_API_NAMESPACE_END
