#include "TradeCommon/TcpClient.h"
#include <boost/asio/connect.hpp>
#include <boost/asio/read.hpp>
#include <boost/asio/write.hpp>
#include <cstdint>
#include <limits>

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

    TcpClient::~TcpClient() {
        stop();
    }

    bool TcpClient::connect(const std::string& address, const unsigned short port) {
        stop();

        boost::system::error_code ec;
        tcp::resolver             resolver(mContext);
        const auto                endpoints = resolver.resolve(address, std::to_string(port), ec);
        if (ec) {
            return false;
        }

        mSocket = std::make_unique<tcp::socket>(mContext);
        boost::asio::connect(*mSocket, endpoints, ec);
        if (ec) {
            mSocket.reset();
            return false;
        }

        mStopRequested = false;
        mConnected     = true;
        mReader        = std::thread([this]() { readLoop(); });
        return true;
    }

    void TcpClient::stop() {
        mStopRequested = true;

        boost::system::error_code ec;
        if (mSocket) {
            mSocket->shutdown(tcp::socket::shutdown_both, ec);
            mSocket->close(ec);
        }

        if (mReader.joinable()) {
            mReader.join();
        }

        mSocket.reset();
        mConnected = false;
    }

    bool TcpClient::sendMessage(const std::string& message) const {
        if (!mConnected || !mSocket) {
            return false;
        }

        std::lock_guard<std::mutex> lock(mWriteMutex);
        return writeFrame(*mSocket, message);
    }

    void TcpClient::readLoop() {
        std::string payload;
        while (!mStopRequested && mSocket && readFrame(*mSocket, payload)) {
            if (onMessage) {
                onMessage(payload);
            }
        }

        const bool shouldNotify = mConnected.exchange(false) && !mStopRequested;
        if (shouldNotify && onDisconnect) {
            onDisconnect();
        }
    }
}
