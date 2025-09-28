#pragma once
#include "UtilsLib/Common.h"
#include <condition_variable>
#include <deque>
#include <map>
#include <thread>
#include <optional>

TRANVANH_NAMESPACE_BEGIN

template <typename Type>
class ThreadSafeQueue {
    std::mutex              m;
    std::condition_variable cv;
    std::deque<Type>        mQueue;
    std::atomic_bool mStop = false;


public:
    void push(const Type& value);
    void push(Type&& value);
    std::optional<Type> pop();
    bool empty() {
        std::lock_guard<std::mutex> lock(m);
        return mQueue.empty();
    }
    void stop();
};

template <typename Type>
void ThreadSafeQueue<Type>::push(const Type& value) {
    std::lock_guard<std::mutex> lock(m);
    mQueue.emplace_back(value);
    cv.notify_one();
}

template <typename Type>
void ThreadSafeQueue<Type>::push(Type&& value) {
    std::lock_guard<std::mutex> lock(m);
    mQueue.emplace_back(std::move(value));
    cv.notify_one();
}

template <typename Type>
std::optional<Type> ThreadSafeQueue<Type>::pop() {
    std::unique_lock<std::mutex> lock(m);
    cv.wait(lock, [this]() {
        return !mQueue.empty() || mStop;
    });
    if(mStop){
        return std::nullopt;
    }
    Type value = mQueue.front();
    mQueue.pop_front();
    return value;
}

template <typename Type>
void ThreadSafeQueue<Type>::stop(){
    mStop = true;
    cv.notify_all();
}

TRANVANH_NAMESPACE_END
