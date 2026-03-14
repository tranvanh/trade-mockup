#pragma once
#include "Toybox/Common.h"
#include "Toybox/ThreadPool.h"
#include <atomic>
#include <forward_list>
#include <functional>
#include <thread>

TOYBOX_NAMESPACE_BEGIN
class Application {
    ThreadPool mThreadPool;

public:
    // \todo make the thread count configuration better
    Application(const int threadCount = std::thread::hardware_concurrency())
        : mThreadPool(threadCount) {}
    virtual ~Application();
    virtual void     run();
    virtual void     stop();
    void             runBackgroundTask(std::function<void()> f);
    std::atomic_bool isRunning = false;
};

TOYBOX_NAMESPACE_END
