#pragma once
#include "UtilsLib/Common.h"
#include <memory>

TRANVANH_NAMESPACE_BEGIN

class NetworkComponent {
protected:
    const int TRY_COUNT                   = 3;
    int           mSocket                     = -1;
    NetworkComponent()                        = default;
    NetworkComponent(const NetworkComponent&) = delete;
    virtual ~NetworkComponent();

public:
    int  getSocket() const;
    bool openSocket();
};

TRANVANH_NAMESPACE_END
