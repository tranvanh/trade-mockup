#pragma once
#include <condition_variable>
#include <deque>
#include <map>
#include <thread>

// UTILSLIB_NAMESPACE_BEGIN

template <typename Type>
class ThreadSafeQueue {
    std::mutex              m;
    std::condition_variable cv;
    std::deque<Type>        mQueue;

public:
    void push(const Type& value);
    Type pop();
    bool empty() { return mQueue.empty(); }
};

template <typename Type>
void ThreadSafeQueue<Type>::push(const Type& value) {
    std::lock_guard<std::mutex> lock(m);
    mQueue.emplace_back(value);
    cv.notify_one();
}

template <typename Type>
Type ThreadSafeQueue<Type>::pop() {
    std::unique_lock<std::mutex> lock(m);
    cv.wait(lock, [this]() {
        return !mQueue.empty();
    });
    Type value = mQueue.front();
    mQueue.pop_front();
    return value;
}

// UTILSLIB_NAMESPACE_END
