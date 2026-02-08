#pragma once
#include "UtilsLib/NetworkComponent.h"

TRANVANH_NAMESPACE_BEGIN

class Client : public NetworkComponent {
    mutable std::mutex mSendLock;

public:
    virtual ~Client() = default;
    bool connectToServer(const char* url, const int port) const;

    // Client send follows a rule of sending a size first and content after
    bool sendMessage(const char* msg) const;
    virtual std::optional<int> openSocket() override;

private:
    bool sendSize(const char* msg) const;
    bool sendContent(const char* msg) const;
};

TRANVANH_NAMESPACE_END
