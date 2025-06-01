#pragma once
#include "CallbackLifetime.h"
#include <mutex>
#include <unordered_map>

using CallbackId         = std::size_t;
static CallbackId nextId = 0;

template <typename Signature>
class CallbackList;

template <typename Ret, typename... Args>
class CallbackList<Ret(Args...)> {
    using Callback = std::function<Ret(Args...)>;

    struct {
        std::unordered_map<CallbackId, Callback> list;
        std::mutex                               lock;
    } mCallbacks;

public:
    void operator()(Args&&... args) {
        for (const auto& callback : mCallbacks.list) {
            callback.second(std::forward<Args>(args)...);
        }
    }

    [[nodiscard]] CallbackLifetime add(Callback callback) {
        CallbackId                   id = nextId++;
        std::unique_lock<std::mutex> lock(mCallbacks.lock);
        mCallbacks.list.insert({ id, std::move(callback) });
        lock.unlock();
        return CallbackLifetime([this, id]() {
            std::lock_guard<std::mutex> guard(mCallbacks.lock);
            mCallbacks.list.erase(id);
        });
    }
};
