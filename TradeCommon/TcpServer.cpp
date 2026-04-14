#include "TradeCommon/TcpServer.h"
#include <boost/asio/read.hpp>
#include <boost/asio/write.hpp>
#include <cstdint>
#include <limits>
#include <vector>

namespace TradeCommon {
    namespace {
        using boost::asio::ip::tcp;

        bool readFrame(tcp::socket& socket, std::string& output) {
            std::uint32_t             length = 0;
            boost::system::error_code ec;
            boost::asio::read(socket, boost::asio::buffer(&length, sizeof(length)), ec);
            if (ec) {
                return false;
            }

            output.resize(length);
            boost::asio::read(socket, boost::asio::buffer(output.data(), output.size()), ec);
            return !ec;
        }

        bool writeFrame(tcp::socket& socket, const std::string& message) {
            if (message.size() > std::numeric_limits<std::uint32_t>::max()) {
                return false;
            }

            const std::uint32_t       length = static_cast<std::uint32_t>(message.size());
            boost::system::error_code ec;
            boost::asio::write(socket, boost::asio::buffer(&length, sizeof(length)), ec);
            if (ec) {
                return false;
            }
            boost::asio::write(socket, boost::asio::buffer(message.data(), message.size()), ec);
            return !ec;
        }
    }

    class TcpServer::Session : public std::enable_shared_from_this<TcpServer::Session> {
        TcpServer&                 mServer;
        ConnectionInfo             mInfo;
        tcp::socket                mSocket;
        std::thread                mReader;
        mutable std::mutex         mWriteMutex;
        std::atomic_bool           mStopped = false;

        void readLoop() {
            std::string payload;
            while (!mStopped && readFrame(mSocket, payload)) {
                if (mServer.onMessage) {
                    mServer.onMessage(mInfo.id, payload);
                }
            }
            mServer.removeSession(mInfo.id, mInfo, true);
        }

    public:
        Session(TcpServer& server, tcp::socket socket, ConnectionInfo info)
            : mServer(server)
            , mInfo(std::move(info))
            , mSocket(std::move(socket)) {}

        ~Session() {
            stop();
        }

        void start() {
            mReader = std::thread([self = shared_from_this()]() { self->readLoop(); });
        }

        void stop() {
            const bool alreadyStopped = mStopped.exchange(true);
            if (!alreadyStopped) {
                boost::system::error_code ec;
                mSocket.shutdown(tcp::socket::shutdown_both, ec);
                mSocket.close(ec);
            }

            if (mReader.joinable()) {
                if (mReader.get_id() == std::this_thread::get_id()) {
                    mReader.detach();
                } else {
                    mReader.join();
                }
            }
        }

        bool send(const std::string& message) {
            if (mStopped) {
                return false;
            }

            std::lock_guard<std::mutex> lock(mWriteMutex);
            return writeFrame(mSocket, message);
        }
    };

    TcpServer::TcpServer(const unsigned short port)
        : mPort(port) {}

    TcpServer::~TcpServer() {
        stop();
    }

    bool TcpServer::start() {
        stop();

        boost::system::error_code ec;
        mAcceptor = std::make_unique<tcp::acceptor>(mContext);
        mAcceptor->open(tcp::v4(), ec);
        if (ec) {
            return false;
        }

        mAcceptor->set_option(tcp::acceptor::reuse_address(true), ec);
        if (ec) {
            return false;
        }

        mAcceptor->bind(tcp::endpoint(tcp::v4(), mPort), ec);
        if (ec) {
            return false;
        }

        mAcceptor->listen(boost::asio::socket_base::max_listen_connections, ec);
        if (ec) {
            return false;
        }

        mRunning      = true;
        mAcceptThread = std::thread([this]() { acceptLoop(); });
        return true;
    }

    void TcpServer::stop() {
        mRunning = false;

        boost::system::error_code ec;
        if (mAcceptor) {
            mAcceptor->cancel(ec);
            mAcceptor->close(ec);
        }

        if (mAcceptThread.joinable()) {
            mAcceptThread.join();
        }

        std::vector<std::shared_ptr<Session>> sessions;
        {
            std::lock_guard<std::mutex> lock(mSessionsMutex);
            sessions.reserve(mSessions.size());
            for (auto& [id, session] : mSessions) {
                (void)id;
                sessions.push_back(session);
            }
            mSessions.clear();
        }

        for (const auto& session : sessions) {
            session->stop();
        }

        mAcceptor.reset();
    }

    bool TcpServer::sendTo(const std::uint64_t connectionId, const std::string& message) {
        std::shared_ptr<Session> session;
        {
            std::lock_guard<std::mutex> lock(mSessionsMutex);
            const auto                  it = mSessions.find(connectionId);
            if (it == mSessions.end()) {
                return false;
            }
            session = it->second;
        }
        return session->send(message);
    }

    void TcpServer::broadcast(const std::string& message) {
        std::vector<std::shared_ptr<Session>> sessions;
        {
            std::lock_guard<std::mutex> lock(mSessionsMutex);
            sessions.reserve(mSessions.size());
            for (const auto& [id, session] : mSessions) {
                (void)id;
                sessions.push_back(session);
            }
        }

        for (const auto& session : sessions) {
            session->send(message);
        }
    }

    void TcpServer::acceptLoop() {
        while (mRunning && mAcceptor) {
            boost::system::error_code ec;
            tcp::socket               socket(mContext);
            mAcceptor->accept(socket, ec);
            if (ec) {
                if (!mRunning) {
                    break;
                }
                continue;
            }

            const auto remoteEndpoint = socket.remote_endpoint(ec);
            if (ec) {
                continue;
            }

            ConnectionInfo info;
            info.id      = mNextConnectionId++;
            info.address = remoteEndpoint.address().to_string();
            info.port    = remoteEndpoint.port();

            auto session = std::make_shared<Session>(*this, std::move(socket), info);
            {
                std::lock_guard<std::mutex> lock(mSessionsMutex);
                mSessions.emplace(info.id, session);
            }

            if (onConnect) {
                onConnect(info);
            }
            session->start();
        }
    }

    void TcpServer::removeSession(const std::uint64_t connectionId, const ConnectionInfo& info, const bool notify) {
        std::shared_ptr<Session> session;
        {
            std::lock_guard<std::mutex> lock(mSessionsMutex);
            const auto                  it = mSessions.find(connectionId);
            if (it == mSessions.end()) {
                return;
            }
            session = it->second;
            mSessions.erase(it);
        }

        session->stop();
        if (notify && onDisconnect) {
            onDisconnect(info);
        }
    }
}
