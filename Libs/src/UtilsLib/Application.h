#pragma once
#include "UtilsLib/Common.h"
#include <atomic>
#include <forward_list>
#include <functional>
#include <thread>

TRANVANH_NAMESPACE_BEGIN
class Application {
    std::forward_list<std::thread> mThreadPool;

public:
    virtual ~Application();
    virtual void     run() = 0;
    void             runBackgroundTask(std::function<void()> f);
    std::atomic_bool isRunning = false;
};

TRANVANH_NAMESPACE_END
