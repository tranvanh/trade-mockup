#pragma once
#include "Toybox/Common.h"
#include <boost/asio.hpp>

TOYBOX_NAMESPACE_BEGIN

class Client {
    boost::asio::io_context      mContext;
    boost::asio::ip::tcp::socket mSocket;

public:
    explicit Client(short port);
    virtual ~Client() = default;
    void run();

    // Client send follows a rule of sending a size first and content after
    void sendMessage(const std::string& msg);
};

TOYBOX_NAMESPACE_END
