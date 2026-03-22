#include "Toybox/ThreadPool.h"
#include <forward_list>
#include <gtest/gtest.h>
#include <thread>

TOYBOX_NAMESPACE_BEGIN

TEST(ThreadPool, noRunCalled) {
    ThreadPool pool(8);
    std::atomic<int> counter(0);
    // Expect these to also run when queued prior to run is called
    for (int i = 0; i < 3; ++i) {
        pool.addTask([&]() {
            ++counter;
        });
    }
    EXPECT_TRUE(counter == 0);
}

TEST(ThreadPool, stopCalledWithoutRun) {
    ThreadPool pool(8);
    pool.stop();
    EXPECT_TRUE(true);
}

TEST(ThreadPool, noStopCalled) {
    ThreadPool pool(8);
    pool.run();
    EXPECT_TRUE(true);
}

TEST(ThreadPool, stopCalled) {
    ThreadPool pool(8);
    pool.run();
    std::this_thread::sleep_for(std::chrono::seconds(2));
    pool.stop();
    EXPECT_TRUE(true);
}

TEST(ThreadPool, finishPendingTasks) {
    ThreadPool pool(8);

    std::atomic<int> counter(0);

    pool.run();
    for (int i = 0; i < 10; ++i) {
        pool.addTask([&]() {
            ++counter;
        });
    }
    // These will be left in the backlog after stop is called, expect the clean up
    for (int i = 0; i < 3; ++i) {
        pool.addTask([&]() {
            std::this_thread::sleep_for(std::chrono::seconds(5));
            ++counter;
        });
    }
    pool.stop();
    EXPECT_TRUE(counter == 13);
}

TEST(ThreadPool, queueBeforeRun) {
    ThreadPool pool(8);

    std::atomic<int> counter(0);
    // Expect these to also run when queued prior to run is called
    for (int i = 0; i < 3; ++i) {
        pool.addTask([&]() {
            ++counter;
        });
    }
    pool.run();
    for (int i = 0; i < 10; ++i) {
        pool.addTask([&]() {
            ++counter;
        });
    }
    pool.stop();
    EXPECT_TRUE(counter == 13);
}

TOYBOX_NAMESPACE_END