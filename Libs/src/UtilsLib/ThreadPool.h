#pragma once
#include "ThreadSafeQueue.h"
#include <atomic>
#include <functional>
#include <vector>

TRANVANH_NAMESPACE_BEGIN

class ThreadPool {
    ThreadSafeQueue<std::function<void()>> mTasksQueue;
    std::deque<std::thread>                mWorkers;
    const int                              mWorkersCount;
    std::atomic_bool                       mIsRunning;

public:
    ThreadPool(const int workersCount);
    ~ThreadPool();
    void addTask(std::function<void()> task);
    void run();
    void stop();
};

TRANVANH_NAMESPACE_END
