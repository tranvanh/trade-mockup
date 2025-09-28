#include "UtilsLib/NetworkUtils.h"
#include "UtilsLib/Logger.h"
#include <thread>
#include <netdb.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

TRANVANH_NAMESPACE_BEGIN

NetworkComponent::~NetworkComponent() {
    if (close(mSocket) < 0) {
        herror(nullptr);
    }
}

int NetworkComponent::getSocket() const {
    return mSocket;
}
bool NetworkComponent::openSocket() {
    auto& logger = Logger::instance();
    for (int i = 0; i < TRY_COUNT; ++i) {
         logger.log(Logger::LogLevel::INFO, "Trying to open socket... [", i + 1, "/", TRY_COUNT, "]");
        if (i > 0) {
            std::this_thread::sleep_for(std::chrono::duration<int, std::milli>(3000));
        }
        if ((mSocket = socket(PF_INET, SOCK_STREAM, 0)) < 0) {
            logger.log(Logger::LogLevel::ERROR, "Failed to open socket");
            continue;
        }
        logger.log(Logger::LogLevel::INFO, "Opened");
        return true;
    }
    return false;
}

TRANVANH_NAMESPACE_END
