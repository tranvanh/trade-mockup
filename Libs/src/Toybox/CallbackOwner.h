#pragma once
#include "Toybox/CallbackLifetime.h"
#include "Toybox/Common.h"
#include <forward_list>

TOYBOX_NAMESPACE_BEGIN

class CallbackOwner {
    std::forward_list<CallbackLifetime> mCallbacks;

public:
    CallbackOwner()  = default;
    ~CallbackOwner() = default;

    void registerCallback(CallbackLifetime&& callback) { mCallbacks.emplace_front(std::move(callback)); }
};

TOYBOX_NAMESPACE_END
