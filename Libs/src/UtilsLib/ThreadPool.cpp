#include "ThreadPool.h"

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
                task();
            }
        });
    }
}

void ThreadPool::stop() {
    mIsRunning = false;
}

void ThreadPool::addTask(std::function<void()> task){
    mTasksQueue.push(std::move(task));
}

TRANVANH_NAMESPACE_END
