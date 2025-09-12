#pragma once
#include "UtilsLib/Common.h"
#include "UtilsLib/CallbackLifetime.h"
#include <forward_list>

TRANVANH_NAMESPACE_BEGIN

class CallbackOwner {
    std::forward_list<CallbackLifetime> mCallbacks;

public:
    CallbackOwner()  = default;
    ~CallbackOwner() = default;

    void registerCallback(CallbackLifetime&& callback) { mCallbacks.emplace_front(std::move(callback)); }
};

TRANVANH_NAMESPACE_END
