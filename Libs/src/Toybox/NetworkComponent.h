#pragma once
#include "Toybox/Common.h"
#include <asio.hpp>

TOYBOX_NAMESPACE_BEGIN

class NetworkComponent : public std::enable_shared_from_this<NetworkComponent> {
public:
    explicit NetworkComponent(asio::ip::tcp::socket socket)
        : mSocket(std::move(socket)) {}

    virtual ~NetworkComponent() = default;
    virtual void start() = 0;
    virtual void read() = 0;
    virtual void write() = 0;

protected:
    asio::streambuf mBuffer;
    asio::ip::tcp::socket     mSocket;
};

TOYBOX_NAMESPACE_END
