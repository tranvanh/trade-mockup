#pragma once
#include <boost/asio.hpp>
#include <atomic>
#include <cstdint>
#include <functional>
#include <memory>
#include <mutex>
#include <string>
#include <thread>
#include <unordered_map>

namespace TradeCommon {

    class TcpServer {
    public:
        struct ConnectionInfo {
            std::uint64_t id      = 0;
            std::string   address;
            unsigned short port   = 0;
        };

    private:
        class Session;

        unsigned short                                            mPort = 0;
        boost::asio::io_context                                   mContext;
        std::unique_ptr<boost::asio::ip::tcp::acceptor>           mAcceptor;
        std::unordered_map<std::uint64_t, std::shared_ptr<Session>> mSessions;
        std::mutex                                                mSessionsMutex;
        std::thread                                               mAcceptThread;
        std::atomic_bool                                          mRunning = false;
        std::atomic_uint64_t                                      mNextConnectionId = 1;

        void acceptLoop();
        void removeSession(std::uint64_t connectionId, const ConnectionInfo& info, bool notify);

    public:
        explicit TcpServer(unsigned short port);
        ~TcpServer();

        std::function<void(const ConnectionInfo&)>        onConnect;
        std::function<void(const ConnectionInfo&)>        onDisconnect;
        std::function<void(std::uint64_t, const std::string&)> onMessage;

        bool start();
        void stop();

        bool sendTo(std::uint64_t connectionId, const std::string& message);
        void broadcast(const std::string& message);
    };
}
