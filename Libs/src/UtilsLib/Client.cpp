#include "UtilsLib/Client.h"
#include "UtilsLib/Logger.h"
#include <cstring>
#include <netdb.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

bool Client::connectToServer(const char* url, const int port)const {
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

// SocketData Client::receive() const {
//     SocketData data;
//     data.size = recv(mSocket, data.buffer, BUFSIZ, 0);
//     return data;
// }

bool Client::sendMessage(const char* msg) const{
    auto& logger = Logger::instance();
    const char* req = "Hello server";
    logger.log(Logger::LogLevel::DEBUG, "Sending ... {", msg, "}");
    if(send(getSocket(), msg, strlen(msg), 0) < 0){
        logger.log(Logger::LogLevel::ERROR, "Error while sending message");
        herror(msg);
        return false;
    }
    logger.log(Logger::LogLevel::DEBUG, "Sent");
    return true;
}