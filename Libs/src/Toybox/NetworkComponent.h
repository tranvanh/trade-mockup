#pragma once
#include "Toybox/Common.h"
#include <boost/asio.hpp>

TOYBOX_NAMESPACE_BEGIN

class NetworkComponent : public std::enable_shared_from_this<NetworkComponent> {
public:
    explicit NetworkComponent(boost::asio::ip::tcp::socket socket)
        : mSocket(std::move(socket)) {}

    virtual ~NetworkComponent() = default;
    virtual void start() = 0;
    virtual void read() = 0;
    virtual void write() = 0;

protected:
    boost::asio::streambuf mBuffer;
    boost::asio::ip::tcp::socket     mSocket;
};

TOYBOX_NAMESPACE_END
