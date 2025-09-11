#include "Server.h"
#include "UtilsLib/Logger.h"
#include <cstring>
#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>
#include <arpa/inet.h>

Server::Server(const AddressType addressType, const std::string& address)
    : mAddressType(addressType)
    , mAddress(address) {}
    
bool Server::startListen(const int port, const std::function<void(const SocketData)>& onReceive) {
    auto& logger = Logger::instance();
    int         clientSocket;
    socklen_t   socketLen;
    sockaddr_in socketAddress;

    memset(&socketAddress, '\0', sizeof(socketAddress));

    socketAddress.sin_family = AF_INET;
    socketAddress.sin_port   = htons(port);

    switch (mAddressType) {
    case AddressType::ANY:
        socketAddress.sin_addr.s_addr = htonl(INADDR_ANY);
        break;
    case AddressType::LOCAL:
        socketAddress.sin_addr.s_addr = htonl(INADDR_LOOPBACK);
        break;
    case AddressType::PUBLIC:
        // \todo check the address is valid
        socketAddress.sin_addr.s_addr = inet_addr(mAddress.c_str());
        break;
    default:
        socketAddress.sin_addr.s_addr = htonl(INADDR_ANY);
    }

    logger.log(Logger::LogLevel::INFO, "Binding...");

    if (bind(mSocket, (struct sockaddr *)&socketAddress, sizeof(socketAddress)) < 0) {
        perror("bind");
        return 2;
    }
    logger.log(Logger::LogLevel::INFO, "Binded");
    logger.log(Logger::LogLevel::INFO, "Start listening");
    constexpr int BACKLOG = 4; // todo improve
    listen(mSocket, BACKLOG);
    logger.log(Logger::LogLevel::INFO, "Listening...");
    while (true) {
        socketLen = sizeof(socketAddress);
        if ((clientSocket = accept(mSocket, (struct sockaddr*)&socketAddress, &socketLen)) < 0) {
            perror("client accept");
            return 4;
        }
        SocketData data;
        if ((data.size = recv(clientSocket, data.buffer, BUFSIZ, 0)) < 0) {
            perror("client recv");
            return 4;
        }
        onReceive(std::move(data));
        close(clientSocket);
    }
    return true;
}