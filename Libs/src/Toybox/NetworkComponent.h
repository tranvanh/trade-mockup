#pragma once
#include "Toybox/Common.h"
#include <asio.hpp>

TOYBOX_NAMESPACE_BEGIN

using asio::ip::tcp;

class Session : public std::enable_shared_from_this<Session> {
public:
    explicit Session(tcp::socket socket)
        : mSocket(std::move(socket)) {}

    virtual ~Session() = default;
    virtual void start() = 0;
    virtual void read() = 0;
    virtual void send() = 0;

protected:
    asio::streambuf mBuffer;

private:
    tcp::socket     mSocket;
};

TOYBOX_NAMESPACE_END
