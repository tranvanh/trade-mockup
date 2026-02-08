#include "UtilsLib/Logger.h"
#include "UtilsLib/NetworkComponent.h"
#include <netdb.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <thread>
#include <unistd.h>
#include <asio.hpp>

TRANVANH_NAMESPACE_BEGIN

NetworkComponent::~NetworkComponent() {
    bool failed = true;
    for (const int& s : mSockets) {
        failed = close(s) < 0;
    }
    if (failed) {
        herror("Failed to close socket");
    }
}

const std::vector<int>& NetworkComponent::getSockets() const {
    return mSockets;
}
std::optional<int> NetworkComponent::openSocket() {
    auto& logger = Logger::instance();
    for (int i = 0; i < TRY_COUNT; ++i) {
        logger.log(Logger::LogLevel::INFO, "Trying to open socket... [", i + 1, "/", TRY_COUNT, "]");
        if (i > 0) {
            std::this_thread::sleep_for(std::chrono::duration<int, std::milli>(3000));
        }
        int openedSocket = -1;
        if ((openedSocket = socket(PF_INET, SOCK_STREAM, 0)) < 0) {
            logger.log(Logger::LogLevel::ERROR, "Failed to open socket");
            continue;
        }
        logger.log(Logger::LogLevel::INFO, "Opened");
        mSockets.push_back(openedSocket);
        return openedSocket;
    }
    return std::nullopt;
}

TRANVANH_NAMESPACE_END
