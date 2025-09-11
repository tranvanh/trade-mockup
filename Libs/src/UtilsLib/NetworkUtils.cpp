#include "UtilsLib/NetworkUtils.h"
#include "UtilsLib/Logger.h"
#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>
#include <netdb.h>

NetworkComponent::~NetworkComponent() {
    if(close(mSocket) < 0){
        herror(nullptr);
    }
}

int NetworkComponent::getSocket()const {return mSocket;}
bool NetworkComponent::openSocket() {
    auto&              logger = Logger::instance();
    logger.log(Logger::LogLevel::INFO, "Openning socket...");
    if ((mSocket = socket(PF_INET, SOCK_STREAM, 0)) < 0) {
        logger.log(Logger::LogLevel::ERROR, "Failed to open socket");
        return false;
    }
    logger.log(Logger::LogLevel::INFO, "Opened");
    return true;
}
