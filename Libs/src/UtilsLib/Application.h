#pragma once
#include "UtilsLib/Common.h"
#include "UtilsLib/ThreadPool.h"
#include <atomic>
#include <forward_list>
#include <functional>
#include <thread>

TRANVANH_NAMESPACE_BEGIN
class Application {
    ThreadPool mThreadPool;

public:
    // \todo make the thread count configuration better
    Application(const int threadCount = std::thread::hardware_concurrency())
        : mThreadPool(threadCount) {}
    virtual ~Application();
    virtual void     run();
    void             runBackgroundTask(std::function<void()> f);
    std::atomic_bool isRunning = false;
};

TRANVANH_NAMESPACE_END
