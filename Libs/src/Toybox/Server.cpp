#include "Toybox/Server.h"
#include "Toybox/Logger.h"
#include "Toybox/NetworkComponent.h"

TOYBOX_NAMESPACE_BEGIN

class Session final : public NetworkComponent {
    struct Message {
        std::vector<char> body   = std::vector<char>(BUFSIZ);
        size_t            length = 0;
    };
    Message mMessage;

public:
    explicit Session(asio::ip::tcp::socket socket)
        : NetworkComponent(std::move(socket)) {}

    virtual ~Session() = default;
    virtual void start() override;
    virtual void read() override;

    virtual void write() override;
    // void send();

    std::function<void(std::string)> onRecieve;

private:
    void readHeader();
    void readBody();
};

void Session::start() {
    read();
}

void Session::read() {
    readHeader();
}

void Session::readHeader() {
    auto self = shared_from_this();

    asio::async_read(mSocket,
                     asio::buffer(&mMessage.length, sizeof(mMessage.length)),
                     [this, self](auto ec, auto) {
                         if (!ec) {
                             mMessage.length = ntohl(mMessage.length);
                             readBody();
                         }
                     });
}

void Session::readBody() {
    auto self = shared_from_this();
    mMessage.body.resize(mMessage.length);

    asio::async_read(mSocket,
                     asio::buffer(mMessage.body),
                     [this, self](asio::error_code ec, [[maybe_unused]] std::size_t length) {
                         if (!ec) {
                             onRecieve(std::string(mMessage.body.data(), mMessage.length));
                             readHeader();
                         }
                     });
}

void Session::write() {}
//
// void Session::send() {
//
// }

// ========================================================
// Server
// ========================================================

Server::Server(short port)
    : mAcceptor(mContext, asio::ip::tcp::endpoint(asio::ip::tcp::v4(), port)) {}

void Server::run() {
    mContext.run();
    accept();
}

void Server::accept() {
    mAcceptor.async_accept([this](asio::error_code ec, asio::ip::tcp::socket socket) {
        if (!ec) {
            std::lock_guard<std::mutex> lock(mActiveSessions.mtx);
            auto session = std::make_shared<Session>(std::move(socket));
            mActiveSessions.data.insert(session);
            session->start();
        }
        accept();
    });
}

TOYBOX_NAMESPACE_END
