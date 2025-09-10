#include "UtilsLib/Client.h"
#include <iostream>
#include <string>
#include <cstring>
#include <string_view>
#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>
#include <netdb.h>
#include "UtilsLib/Logger.h"

Client::~Client() {
    if(close(mSocket) < 0){
        herror(nullptr);
    }
}

bool Client::openSocket() {
    auto&              logger = Logger::instance();
    logger.log(Logger::LogLevel::INFO, "Openning socket...");
    if ((mSocket = socket(PF_INET, SOCK_STREAM, 0)) < 0) {
        logger.log(Logger::LogLevel::ERROR, "Failed to open socket");
        return false;
    }
    logger.log(Logger::LogLevel::INFO, "Opened");
    return true;
}

bool Client::connectToServer(const std::string& url){
    Logger&              logger = Logger::instance();
    sockaddr_in socketAddress;
    hostent* host;
    std::memset(&socketAddress, '\0', sizeof(socketAddress));

    socketAddress.sin_family      = AF_INET;
    socketAddress.sin_port        = htons(13);
    if ((host = gethostbyname(url.c_str())) == NULL) {
        logger.log(Logger::LogLevel::ERROR, "Failed to connect to server");
        herror(url.c_str());
        return 2;
    }
    memcpy(&socketAddress.sin_addr, host->h_addr_list[0], host->h_length);
    
    // \todo handle timeout or unsuccesful connection
    logger.log(Logger::LogLevel::INFO, "Connecting to the server...");
    if (connect(mSocket, (sockaddr*)&socketAddress, sizeof(socketAddress)) < 0) {
        logger.log(Logger::LogLevel::ERROR, "Error while connecting to the address");
        close(mSocket);
        return 2;
    }
    logger.log(Logger::LogLevel::INFO, "Connected");
    return true;
}

SocketData Client::receive() const {
    SocketData data;
    data.size = recv(mSocket, data.buffer, BUFSIZ, 0);
    return data;
}