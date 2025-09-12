#include "UtilsLib/Server.h"
#include "UtilsLib/Logger.h"
#include <arpa/inet.h>
#include <cstring>
#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

TRANVANH_NAMESPACE_BEGIN

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
        return false;
    }
    // debug only end
    if (bind(mSocket, (struct sockaddr*)&socketAddress, sizeof(socketAddress)) < 0) {
        perror("bind");
        return false;
    }
    logger.log(Logger::LogLevel::INFO, "Binded");
    logger.log(Logger::LogLevel::INFO, "Start listening");
    constexpr int BACKLOG = 4; // todo improve
    listen(mSocket, BACKLOG);
    logger.log(Logger::LogLevel::INFO, "Listening...");
    socketLen = sizeof(socketAddress);
    if ((clientSocket = accept(mSocket, (struct sockaddr*)&socketAddress, &socketLen)) < 0) {
        perror("client accept");
        return false;
    }
    if (!receive(clientSocket, onReceive)) {
        logger.log(Logger::LogLevel::ERROR, "Error while receiving a message");
        perror("receive error");
        return false;
    }
    return true;
}

bool Server::receive(const int                                         clientSocket,
                     std::function<void(std::vector<char>, const int)> onReceive) const {
    auto& logger     = Logger::instance();
    std::vector<char> buffer(BUFSIZ);
    int               recvLength = -1;
    logger.log(Logger::LogLevel::DEBUG, "Start receiving.");
    while (true) {
        size_t msgLen = 0;
        if (!receiveSize(clientSocket, msgLen) || !receiveContent(clientSocket, msgLen, buffer)) {
            return false;
        }
        onReceive(std::vector<char>(buffer.begin(), buffer.begin() + msgLen), msgLen);
    }
    return true;
}

bool Server::receiveSize(const int clientSocket, size_t& out) const {
    auto& logger     = Logger::instance();
    int   recvLength = -1;
    if ((recvLength = recv(clientSocket, &out, sizeof(size_t), 0)) < 0) {
        return false;
    }
    if (recvLength == 0) {
        return false;
    }
    logger.log(Logger::LogLevel::DEBUG, "Received message size [", out, "]");
    return true;
}

bool Server::receiveContent(const int clientSocket, const size_t msgLen, std::vector<char>& out) const {
    auto&  logger            = Logger::instance();
    size_t received          = 0;
    int    receiveBufferSize = BUFSIZ < msgLen ? BUFSIZ : msgLen;
    size_t recvLength        = -1;
    logger.log(Logger::LogLevel::DEBUG, "Start receiving, receiveBuffer ", receiveBufferSize);
    while (received < msgLen) {
        logger.log(Logger::LogLevel::DEBUG, "Receiving...", receiveBufferSize);
        if ((recvLength = recv(clientSocket, out.data(), receiveBufferSize, 0)) < 0) {
            perror("client size recv");
            return false;
        }
        if (recvLength == 0) {
            return false;
        }
        logger.log(Logger::LogLevel::DEBUG, "Received [", recvLength, "/", msgLen, "]");
        received += recvLength;
        receiveBufferSize = BUFSIZ < (msgLen-received) ? BUFSIZ : (msgLen-received);
    }
    return true;
}

TRANVANH_NAMESPACE_END
