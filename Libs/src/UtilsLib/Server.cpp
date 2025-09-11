#include "UtilsLib/Server.h"
#include "UtilsLib/Logger.h"
#include <arpa/inet.h>
#include <cstring>
#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

Server::Server(const AddressType addressType, const std::string& address)
    : mAddressType(addressType)
    , mAddress(address) {}

bool Server::startListen(const int port, std::function<void(std::vector<char>, const int)> onReceive) {
    auto&       logger = Logger::instance();
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
    // \todo make this debug only begin
    int yes = 1;
    if (setsockopt(mSocket, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(yes)) < 0) {
        perror("setsockopt");
    }
    // debug only end
    if (bind(mSocket, (struct sockaddr*)&socketAddress, sizeof(socketAddress)) < 0) {
        perror("bind");
        return 2;
    }
    logger.log(Logger::LogLevel::INFO, "Binded");
    logger.log(Logger::LogLevel::INFO, "Start listening");
    constexpr int BACKLOG = 4; // todo improve
    listen(mSocket, BACKLOG);
    logger.log(Logger::LogLevel::INFO, "Listening...");
    socketLen = sizeof(socketAddress);
    if ((clientSocket = accept(mSocket, (struct sockaddr*)&socketAddress, &socketLen)) < 0) {
        perror("client accept");
        return 4;
    }
    std::vector<char> buffer(BUFSIZ);
    int recvLength = -1;
    while (true) {
        if ((recvLength = recv(clientSocket, buffer.data(), BUFSIZ, 0)) < 0) {
            perror("client recv");
            return 4;
        }
        if(recvLength == 0){
            return true;
        }
        onReceive(std::vector<char>(buffer.begin(), buffer.begin()+ recvLength), recvLength);
    }
    return true;
}