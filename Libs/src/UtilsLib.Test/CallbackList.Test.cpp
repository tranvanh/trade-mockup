#include <gtest/gtest.h>
#include <forward_list>
#include "UtilsLib/ThreadSafeQueue.h"
#include "UtilsLib/CallbackList.h"

TRANVANH_NAMESPACE_BEGIN

TEST(CallbackList, lifetime) {
    CallbackList<void()> callbackList;
    int result = 0;
    CallbackLifetime lifetime1 = callbackList.add([&result](){++result;});
    {
        CallbackLifetime lifetime2 = callbackList.add([&result](){++result;});
        callbackList();
        EXPECT_EQ(result, 2);
    }
    callbackList();
    EXPECT_EQ(result, 3);
}

TEST(CallbackList, addMultiThread) {
    std::forward_list<std::thread> threads;
    CallbackList<void()> callbackList;
    const int                      numThreads = 3;
    std::atomic<int>               result = 0;
    ThreadSafeQueue<CallbackLifetime> lifetimes;
    for (int i = 0; i < numThreads; ++i) {
        threads.emplace_front([&result, &callbackList, &lifetimes]() {
           lifetimes.push(callbackList.add([&result](){++result;}));
        });
    }
    std::this_thread::sleep_for(std::chrono::duration<int, std::milli>(2000));
    callbackList();
    for (auto& t : threads) {
        t.join();
    }
    EXPECT_EQ(result, 3);
}

TEST(CallbackList, operatorMultiThread) {
    std::forward_list<std::thread> threads;
    CallbackList<void()>           callbackList;
    const int                      numThreads = 3;
    std::atomic<int>               result       = 0;
    CallbackLifetime               lifetime     = callbackList.add([&result]() {
        ++result;
    });
    for (int i = 0; i < numThreads; ++i) {
        threads.emplace_front([&result, &callbackList]() {
           callbackList();
        });
    }
    std::this_thread::sleep_for(std::chrono::duration<int, std::milli>(1000));
    for (auto& t : threads) {
        t.join();
    }
    EXPECT_EQ(result, 3);
}

TRANVANH_NAMESPACE_END