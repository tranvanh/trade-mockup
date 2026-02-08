#include "UtilsLib/Client.h"
#include "UtilsLib/Logger.h"
#include <cstring>
#include <netdb.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <thread>
#include <unistd.h>

TRANVANH_NAMESPACE_BEGIN

std::optional<int> Client::openSocket() {
    if (mSockets.size() > 0) {
        Logger& logger = Logger::instance();
        logger.log(Logger::LogLevel::ERROR, "Can't open more than 1 socket for a client");
        return std::nullopt;
    }
    return NetworkComponent::openSocket();
}

bool Client::connectToServer(const char* url, const int port) const {
    ASSERT(!mSockets.empty(), "No socket is openned");
    ASSERT(mSockets.size() > 1, "Client has more than one socket openned");

    Logger&     logger = Logger::instance();
    sockaddr_in socketAddress;
    hostent*    host;
    std::memset(&socketAddress, '\0', sizeof(socketAddress));

    socketAddress.sin_family = AF_INET;
    socketAddress.sin_port   = htons(port);
    if ((host = gethostbyname(url)) == NULL) {
        logger.log(Logger::LogLevel::ERROR, "Failed to connect to server");
        herror(url);
        return 2;
    }
    memcpy(&socketAddress.sin_addr, host->h_addr_list[0], host->h_length);
    for (int i = 0; i < TRY_COUNT; ++i) {
        logger.log(Logger::LogLevel::INFO, "Trying to connect... [", i + 1, "/", TRY_COUNT, "]");
        if (i > 0) {
            std::this_thread::sleep_for(std::chrono::duration<int, std::milli>(3000));
        }
        if (connect(mSockets[0], (sockaddr*)&socketAddress, sizeof(socketAddress)) < 0) {
            logger.log(Logger::LogLevel::ERROR, "Error while connecting");
            continue;
        }
        logger.log(Logger::LogLevel::INFO, "Connected");
        return true;
    }
    close(mSockets[0]);
    return false;
}

bool Client::sendMessage(const char* msg) const {
    auto& logger = Logger::instance();
    logger.log(Logger::LogLevel::DEBUG, "Sending ... ", msg, " | len = ", strlen(msg));
    std::lock_guard<std::mutex> lock(mSendLock);
    if (!sendSize(msg) || !sendContent(msg)) {
        logger.log(Logger::LogLevel::ERROR, "Error while sending message");
        herror(msg);
        return false;
    }
    logger.log(Logger::LogLevel::DEBUG, "Sent");
    return true;
}

bool Client::sendSize(const char* msg) const {
    size_t msgSize = strlen(msg);
    if (send(mSockets[0], &msgSize, sizeof(msgSize), 0) < 0) {
        return false;
    }
    return true;
}

bool Client::sendContent(const char* msg) const {
    if (send(mSockets[0], msg, strlen(msg), 0) < 0) {
        return false;
    }
    return true;
}

TRANVANH_NAMESPACE_END
