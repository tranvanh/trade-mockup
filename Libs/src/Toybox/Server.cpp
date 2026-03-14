#include "Toybox/Server.h"
#include "Toybox/Logger.h"
#include "Toybox/NetworkComponent.h"

TOYBOX_NAMESPACE_BEGIN

class Session final : public NetworkComponent {
    const Server& mServer;
    struct {
        std::vector<char> body   = std::vector<char>(BUFSIZ);
        std::size_t            length = 0;
    } mMessage;

public:
    explicit Session(boost::asio::ip::tcp::socket socket, const Server& server)
        : NetworkComponent(std::move(socket)), mServer(server) {}

    virtual ~Session() = default;
    virtual void start() override;
    virtual void read() override;

    virtual void write() override;
    // void send();


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
    boost::asio::async_read(mSocket,
                     boost::asio::buffer(&mMessage.length, sizeof(mMessage.length)),
                     [this, self](boost::system::error_code ec, std::size_t) {
                         if (!ec) {
                             readBody();
                         }
                     });
}

void Session::readBody() {
    auto self = shared_from_this();
    mMessage.body.resize(mMessage.length);

    boost::asio::async_read(mSocket,
                     boost::asio::buffer(mMessage.body, mMessage.length*sizeof(char)),
                     [this, self](boost::system::error_code ec, [[maybe_unused]] std::size_t length) {
                         if (!ec) {
                             mServer.onRecieve(std::string(mMessage.body.data(), mMessage.length));
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
    : mAcceptor(mContext, boost::asio::ip::tcp::endpoint(boost::asio::ip::tcp::v4(), port)) {}

Server::~Server() {
    mContext.stop();
}

void Server::run() {
    accept();
    mContext.run();
}

void Server::accept() {
    mAcceptor.async_accept([this](boost::system::error_code ec, boost::asio::ip::tcp::socket socket) {
        if (!ec) {
            // Get identification info
            const auto           remote_endpoint = socket.remote_endpoint();
            const std::string    address         = remote_endpoint.address().to_string();
            const unsigned short port            = remote_endpoint.port();
            Logger::instance().log(Logger::LogLevel::INFO,
                                   "New connection from " + address + ":" + std::to_string(port));
            std::lock_guard<std::mutex> lock(mActiveSessions.mtx);
            auto session = std::make_shared<Session>(std::move(socket), *this);
            mActiveSessions.data.insert(session);
            session->start();
        }
        accept();
    });
}

TOYBOX_NAMESPACE_END
