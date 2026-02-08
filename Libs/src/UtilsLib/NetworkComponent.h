#pragma once
#include "UtilsLib/Common.h"
#include <vector>
#include <optional>

TRANVANH_NAMESPACE_BEGIN

class NetworkComponent {
protected:
    const int TRY_COUNT                       = 3;
    std::vector<int>       mSockets;
    NetworkComponent()                        = default;
    virtual ~NetworkComponent();

public:
    NetworkComponent(const NetworkComponent&) = delete;
    const std::vector<int>&  getSockets() const;

    // Returns the socket number if succesful
    virtual std::optional<int> openSocket();
};

TRANVANH_NAMESPACE_END
