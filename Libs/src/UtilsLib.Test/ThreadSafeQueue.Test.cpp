#include "UtilsLib/ThreadSafeQueue.h"
#include <forward_list>
#include <gtest/gtest.h>
#include <thread>

TRANVANH_NAMESPACE_BEGIN

TEST(ThreadSafeQueue, non_blocking_pop) {
    ThreadSafeQueue<int> tsQueue;
    tsQueue.try_pop();
    EXPECT_TRUE(true);
}

TEST(ThreadSafeQueue, orderSingleThread) {
    ThreadSafeQueue<int> tsQueue;
    tsQueue.push(1);
    tsQueue.push(2);
    tsQueue.push(3);
    EXPECT_FALSE(tsQueue.empty());
    EXPECT_EQ(tsQueue.pop(), 1);
    EXPECT_EQ(tsQueue.pop(), 2);
    EXPECT_EQ(tsQueue.pop(), 3);
    EXPECT_TRUE(tsQueue.empty());
}

TEST(ThreadSafeQueue, orderMultiThread) {
    ThreadSafeQueue<int> tsQueue;
    std::thread          t([&tsQueue]() {
        std::this_thread::sleep_for(std::chrono::duration<int, std::milli>(2000));
        tsQueue.push(3);
    });
    EXPECT_TRUE(tsQueue.empty());
    tsQueue.push(1);
    tsQueue.push(2);
    EXPECT_FALSE(tsQueue.empty());
    EXPECT_EQ(tsQueue.pop(), 1);
    EXPECT_EQ(tsQueue.pop(), 2);
    EXPECT_EQ(tsQueue.pop(), 3);
    EXPECT_TRUE(tsQueue.empty());
    EXPECT_TRUE(tsQueue.empty());
    t.join();
}

// TEST(ThreadSafeQueue, multiConsumerProducer) {
//     ThreadSafeQueue<int>           queue;
//     std::forward_list<std::thread> threads;
//     const int                      numProcuders = 3;
//     const int                      numConsumers = 3;
//     std::atomic<int>               result;
//     for (int i = 0; i < numProcuders; ++i) {
//         threads.emplace_front([&queue, i]() {
//             queue.push(i + 1);
//         });
//     }
//     for (int i = 0; i < numConsumers; ++i) {
//         threads.emplace_front([&queue, &result]() {
//             result += queue.pop();
//         });
//     }
//     for (auto& t : threads) {
//         t.join();
//     }
//     EXPECT_TRUE(queue.empty());
//     EXPECT_EQ(result, 6);
// }

TRANVANH_NAMESPACE_END
