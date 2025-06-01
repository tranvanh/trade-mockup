#pragma once
#include <forward_list>
#include "CallbackLifetime.h"

class CallbackOwner{
    std::forward_list<CallbackLifetime> mCallbacks;
public:
    CallbackOwner()= default;
    ~CallbackOwner() = default;

    void registerCallback(CallbackLifetime&& callback){
         mCallbacks.emplace_front(std::move(callback));
    }
};