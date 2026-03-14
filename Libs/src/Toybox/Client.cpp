#include "Toybox/Client.h"
#include "Toybox/Logger.h"

TOYBOX_NAMESPACE_BEGIN

Client::Client() : mSocket(mContext) {
}

Client::~Client() {
    mContext.stop();
}

bool Client::connect(const std::string& address, short port){
    boost::system::error_code ec;
    mSocket.connect(boost::asio::ip::tcp::endpoint(boost::asio::ip::make_address(address), port), ec);
    return ec == boost::system::errc::success;
}

void Client::run() {
    mContext.run();
}

void Client::sendMessage(const std::string& msg) const{
    const auto len = msg.size();

    mBuffer.clear();
    mBuffer.emplace_back(boost::asio::buffer(&len, sizeof(len)));
    mBuffer.emplace_back(boost::asio::buffer(msg));

    // guaranteed to send complete buffers
    boost::asio::async_write(
        mSocket,
        mBuffer,
        [](std::error_code ec, std::size_t)
        {
            if (ec) {
                Logger::instance().log(Logger::LogLevel::ERROR, "Send failed");
            }
        });
}

TOYBOX_NAMESPACE_END
