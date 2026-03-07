#include "Toybox/Client.h"
#include "Toybox/Logger.h"

TOYBOX_NAMESPACE_BEGIN

Client::Client(short port) : mSocket(mContext) {
    mSocket.connect(asio::ip::tcp::endpoint(asio::ip::tcp::v4(), port));
};

void Client::run() {
    mContext.run();
}

void Client::sendMessage(const std::string& msg) {
    uint32_t len = htonl(static_cast<uint32_t>(msg.size()));

    std::vector<asio::const_buffer> buffers;
    buffers.emplace_back(asio::buffer(&len, sizeof(len)));
    buffers.emplace_back(asio::buffer(msg));

    // guaranteed to send complete buffers
    asio::async_write(
        mSocket,
        buffers,
        [](std::error_code ec, std::size_t)
        {
            if (ec) {
                Logger::instance().log(Logger::LogLevel::ERROR, "Send failed");
            }
        });
}

TOYBOX_NAMESPACE_END
