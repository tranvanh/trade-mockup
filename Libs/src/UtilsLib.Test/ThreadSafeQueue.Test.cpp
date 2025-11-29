#include "UtilsLib/ThreadSafeQueue.h"
#include <forward_list>
#include <gtest/gtest.h>
#include <thread>

TRANVANH_NAMESPACE_BEGIN

TEST(ThreadSafeQueue, orderSingleThread) {
    ThreadSafeQueue<int> queue;
    queue.push(1);
    queue.push(2);
    queue.push(3);
    EXPECT_FALSE(queue.empty());
    EXPECT_EQ(queue.pop(), 1);
    EXPECT_EQ(queue.pop(), 2);
    EXPECT_EQ(queue.pop(), 3);
    EXPECT_TRUE(queue.empty());
}

TEST(ThreadSafeQueue, orderMultiThread) {
    ThreadSafeQueue<int> queue;
    std::thread          t([&queue]() {
        std::this_thread::sleep_for(std::chrono::duration<int, std::milli>(2000));
        queue.push(3);
    });
    EXPECT_TRUE(queue.empty());
    queue.push(1);
    queue.push(2);
    EXPECT_FALSE(queue.empty());
    EXPECT_EQ(queue.pop(), 1);
    EXPECT_EQ(queue.pop(), 2);
    EXPECT_EQ(queue.pop(), 3);
    EXPECT_TRUE(queue.empty());
    EXPECT_TRUE(queue.empty());
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
