#pragma once
#include <unordered_map>
#include <CallbackLifetime.h>

using CallbackId         = std::size_t;
static CallbackId nextId = 0;

template <typename Signature>
class CallbackList;

template <typename Ret, typename... Args>
class CallbackList<Ret(Args...)> {
    using Callback = std::function<Ret(Args...)>;

    std::unordered_map<CallbackId, Callback> mCallbacks;

public:
    void operator()(Args&&... args) {
        for (const auto& callback : mCallbacks) {
            callback.second(std::forward<Args>(args)...);
        }
    }

    [[nodiscard]] CallbackLifetime add(Callback callback) {
        CallbackId id = nextId++;
        mCallbacks.insert({ id, std::move(callback) });
        return CallbackLifetime([this, id]() {
            mCallbacks.erase(id);
        });
    }
};
