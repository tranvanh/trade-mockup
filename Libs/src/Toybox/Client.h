#pragma once
#include "Toybox/Common.h"
#include <boost/asio.hpp>

TOYBOX_NAMESPACE_BEGIN

class Client {
    boost::asio::io_context      mContext;
    mutable boost::asio::ip::tcp::socket mSocket;
    mutable std::vector<boost::asio::const_buffer> mBuffer;

public:
    Client();
    ~Client();
    void run();
    bool connect(const std::string& address, short port );
    // Client send follows a rule of sending a size first and content after
    void sendMessage(const std::string& msg) const;
};

TOYBOX_NAMESPACE_END
