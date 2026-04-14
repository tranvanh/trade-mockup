#pragma once
#include <boost/asio.hpp>
#include <atomic>
#include <functional>
#include <memory>
#include <mutex>
#include <string>
#include <thread>

namespace TradeCommon {

    class TcpClient {
        boost::asio::io_context                           mContext;
        std::unique_ptr<boost::asio::ip::tcp::socket>    mSocket;
        mutable std::mutex                               mWriteMutex;
        std::thread                                      mReader;
        std::atomic_bool                                 mConnected     = false;
        std::atomic_bool                                 mStopRequested = false;

        void readLoop();

    public:
        std::function<void(const std::string&)> onMessage;
        std::function<void()>                   onDisconnect;

        TcpClient() = default;
        ~TcpClient();

        bool connect(const std::string& address, unsigned short port);
        void stop();

        [[nodiscard]] bool sendMessage(const std::string& message) const;
        [[nodiscard]] bool isConnected() const { return mConnected.load(); }
    };
}
