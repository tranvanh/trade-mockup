#include "UtilsLib/ThreadPool.h"

TRANVANH_NAMESPACE_BEGIN

ThreadPool::ThreadPool(const int workersCount)
    : mWorkersCount(workersCount)
    , mIsRunning(false) {}

ThreadPool::~ThreadPool() {
    for (auto& worker : mWorkers) {
        worker.join();
    }
}

void ThreadPool::run() {
    mIsRunning = true;
    for (int i = 0; i < mWorkersCount; ++i) {
        mWorkers.emplace_back([this]() {
            while (mIsRunning) {
                auto task = mTasksQueue.pop();
                if(!mIsRunning){
                    return;
                }
                ASSERT(task.has_value(), "Invalid task value");
                (*task)();
            }
        });
    }
}

void ThreadPool::stop() {
    mIsRunning = false;
    mTasksQueue.stop();
}

void ThreadPool::addTask(const std::function<void()>& task){
    mTasksQueue.push(task);
}

void ThreadPool::addTask(std::function<void()>&& task){
    mTasksQueue.push(std::move(task));
}

TRANVANH_NAMESPACE_END
