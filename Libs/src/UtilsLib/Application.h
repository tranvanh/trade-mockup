#pragma once
#include <atomic>
#include <forward_list>
#include <functional>
#include <thread>

class Application {
    std::forward_list<std::thread> mThreadPool;

public:
    virtual ~Application();
    virtual void     run() = 0;
    void             runBackgroundTask(std::function<void()> f);
    std::atomic_bool isRunning = false;
};