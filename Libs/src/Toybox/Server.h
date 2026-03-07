#pragma once
#include "Toybox/Common.h"
#include <asio.hpp>
#include <unordered_set>

TOYBOX_NAMESPACE_BEGIN

class Session;

class Server {
    asio::io_context                    mContext;
    asio::ip::tcp::acceptor              mAcceptor;
    struct {
        std::unordered_set<std::shared_ptr<Session>> data;
        mutable std::mutex                           mtx;
    } mActiveSessions;

public:
    explicit Server(short port);
    void run();

private:
    void accept();

};

TOYBOX_NAMESPACE_END
