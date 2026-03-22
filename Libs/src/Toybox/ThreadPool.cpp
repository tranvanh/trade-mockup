#include "Toybox/ThreadPool.h"

TOYBOX_NAMESPACE_BEGIN

ThreadPool::ThreadPool(const int workersCount)
    : mWorkersCount(workersCount)
    , mIsRunning(false) {}

ThreadPool::~ThreadPool() {
    // For workers to not be cleared only means, that stop was not previously called
    if (!mWorkers.empty()) {
        stop();
    }
}

void ThreadPool::run() {
    mIsRunning = true;
    for (int i = 0; i < mWorkersCount; ++i) {
        mWorkers.emplace_back([this]() {
            while (mIsRunning) {
                auto task = mTasksQueue.pop();
                if (task) {
                    (*task)();
                }
            }
        });
    }
}

void ThreadPool::stop() {
    if (mWorkers.empty()) {
        return;
    }
    mIsRunning = false;
    mTasksQueue.stop();
    for (auto& worker : mWorkers) {
        worker.join();
    }
    mWorkers.clear();
    while (!mTasksQueue.empty()) {
        auto task = mTasksQueue.pop();
        if (task) {
            (*task)();
        }
    }
}

void ThreadPool::addTask(const std::function<void()>& task){
    mTasksQueue.push(task);
}

void ThreadPool::addTask(std::function<void()>&& task){
    mTasksQueue.push(std::move(task));
}

TOYBOX_NAMESPACE_END
