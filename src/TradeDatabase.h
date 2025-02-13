#pragma once

#include "Model.h"
#include <thread>
#include <condition_variable>
#include <list>
#include <unordered_map>

TRADE_API_NAMESPACE_BEGIN

class TradeApp;

template <typename Type>
class ThreadSafeQueue {
    std::mutex m;
    std::condition_variable cv;
    std::list<Type> mQueue;

public:
    void push(const Type& value);
    Type pop();
    bool empty(){return mQueue.empty();}
};


// must be thread safe queue
// producer consumer
// thread safe database
// - unordered map
class TradeDatabase {
    TradeApp& mApplication;
    ThreadSafeQueue<Trade> mQueue;
    std::unordered_map<uint64_t, Trade> mData;

public:
    TradeDatabase(TradeApp& app);
    void registerTrade(const Trade& t);
    void run();
    // store all trades
    // search by id
    // search by timestamp
};

TRADE_API_NAMESPACE_END
