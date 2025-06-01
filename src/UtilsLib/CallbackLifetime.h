#pragma once
#include <functional>

class CallbackLifetime {
    std::function<void()> mRemoveCallback;

public:
    // Disallow copy construction
    CallbackLifetime(const CallbackLifetime&) = delete;
    CallbackLifetime(CallbackLifetime&& other) { mRemoveCallback = std::move(other.mRemoveCallback); }
    CallbackLifetime(const std::function<void()>& removeCallback)
        : mRemoveCallback(std::move(removeCallback)) {}
    ~CallbackLifetime() {
        // remove callback can become null due to std::moves
        if (mRemoveCallback) {
            mRemoveCallback();
            mRemoveCallback = nullptr;
        }
    }
};