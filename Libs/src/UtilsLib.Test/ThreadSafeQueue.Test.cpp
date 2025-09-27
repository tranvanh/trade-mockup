#include "UtilsLib/ThreadSafeQueue.h"
#include <gtest/gtest.h>
#include <thread>
#include <forward_list>

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
    ThreadSafeQueue<int>           queue;
    std::forward_list<std::thread> threads;
    std::thread                    t([&queue]() {
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

TRANVANH_NAMESPACE_END
