#pragma once
#include "Toybox/NetworkComponent.h"

TOYBOX_NAMESPACE_BEGIN

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

TOYBOX_NAMESPACE_END
