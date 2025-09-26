#include "UtilsLib/Server.h"
#include "UtilsLib/Logger.h"
#include <arpa/inet.h>
#include <cstring>
#include <fcntl.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

#if defined(__linux__)
#   include <sys/epoll.h>
#endif

TRANVANH_NAMESPACE_BEGIN

#define MAX_EVENTS 10 // \todo tweak

Server::Server(const AddressType addressType, const std::string& address)
    : mAddressType(addressType)
    , mAddress(address) {}

bool Server::startListen(const int port, std::function<void(std::vector<char>, const int)> onReceive) {
    auto&       logger = Logger::instance();
    sockaddr_in socketAddress;
    socklen_t   socketLen;

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
    logger.log(Logger::LogLevel::INFO, "Start listening on port:", port);
    constexpr int BACKLOG = 4; // todo improve
    listen(mSocket, BACKLOG);
    logger.log(Logger::LogLevel::INFO, "Listening...");
    logger.log(Logger::LogLevel::INFO, "Start polling events");
    poll(socketAddress, socketLen, std::move(onReceive));
    return true;
}

bool Server::poll(sockaddr_in                                       socketAddress,
                  socklen_t                                         socketLen,
                  std::function<void(std::vector<char>, const int)> onReceive) {
#if defined(__linux__)
    auto&       logger = Logger::instance();
    epoll_event ev, events[MAX_EVENTS];
    int         epollfd = epoll_create1(0);
    if (epollfd == -1) {
        perror("epoll_create1");
        return false;
    }
    ev.events  = EPOLLIN;
    ev.data.fd = mSocket;
    if (epoll_ctl(epollfd, EPOLL_CTL_ADD, mSocket, &ev) == -1) {
        perror("epoll_ctl: listen_sock");
        return false;
    }

    int clientSocket = 0;
    logger.log(Logger::LogLevel::INFO, "Polling...");
    while (true) {
        int eventCount = epoll_wait(epollfd, events, MAX_EVENTS, -1);
        if (eventCount == -1) {
            perror("epoll_wait");
            return false;
        }

        for (int i = 0; i < eventCount; ++i) {
            if (events[i].data.fd == mSocket) {
                // Accept new client
                if ((clientSocket = accept(mSocket, (sockaddr*)&socketAddress, &socketLen)) < 0) {
                    perror("accept");
                    return false;
                }
                if (!setNonBlockingSocket(clientSocket)) {
                    logger.log(Logger::LogLevel::ERROR,
                               "Failed to set nonblocking flag to socket ",
                               clientSocket);
                    perror("set non blocking");
                    continue;
                }
                ev.events  = EPOLLIN;
                ev.data.fd = clientSocket;
                if (epoll_ctl(epollfd, EPOLL_CTL_ADD, clientSocket, &ev) == -1) {
                    perror("epoll_ctl: client socket");
                    continue;
                }
                mClientConnections.insert({ clientSocket, ClientConnection() });
            } else if (events[i].events & EPOLLIN) {
                // Receive data event
                receive(events[i].data.fd, onReceive);
            }
        }
    }
    return true;
// Linux-specific code
#else
    ASSERT(false, "Missing implementation for given os");
    // Other OS (e.g. Windows, BSD, etc.)
    return false;
#endif
}

bool Server::receive(const int clientSocket, std::function<void(std::vector<char>, const int)> onReceive) {
    auto&             logger = Logger::instance();
    std::vector<char> buffer(BUFSIZ);
    int               recvLength = -1;
    logger.log(Logger::LogLevel::DEBUG, "Start receiving.");
    auto clientIter = mClientConnections.find(clientSocket);
    ASSERT(clientIter != mClientConnections.end(), "Client socket was not found among accepted");
    ClientConnection& clientConnetion = clientIter->second;

    bool success = true;
    switch (clientConnetion.state) {
    case ClientConnection::State::WAITING: {
        success = receiveSize(clientSocket, clientConnetion.msgLen);
        if (success) {
            clientConnetion.state = ClientConnection::State::SIZE_RECEIVED;
        }
        break;
    }
    case ClientConnection::State::SIZE_RECEIVED: {
        size_t msgLen = clientConnetion.msgLen;
        success       = receiveContent(clientSocket, msgLen);
        if (success) {
            onReceive(std::vector<char>(mReceiveBuffer.begin(), mReceiveBuffer.begin() + msgLen), msgLen);
        }
        clientConnetion.state = ClientConnection::State::WAITING;
        break;
    }
    }
    return true;
}

bool Server::receiveSize(const int clientSocket, size_t& out) {
    auto& logger     = Logger::instance();
    int   recvLength = -1;
    if ((recvLength = recv(clientSocket, &out, sizeof(size_t), 0)) < 0) {
        return false;
    }
    if (recvLength == 0) {
        logger.log(Logger::LogLevel::WARNING, "Client [", clientSocket, "] disconnected");
        mClientConnections.erase(clientSocket);
        close(clientSocket);
        return false;
    }
    logger.log(Logger::LogLevel::DEBUG, "Received message size [", out, "]");
    return true;
}

bool Server::receiveContent(const int clientSocket, const size_t msgLen) {
    auto&  logger            = Logger::instance();
    size_t received          = 0;
    int    receiveBufferSize = BUFSIZ < msgLen ? BUFSIZ : msgLen;
    size_t recvLength        = -1;
    logger.log(Logger::LogLevel::DEBUG, "Start receiving, receiveBuffer ", receiveBufferSize);
    while (received < msgLen) {
        logger.log(Logger::LogLevel::DEBUG, "Receiving...", receiveBufferSize);
        if ((recvLength = recv(clientSocket, mReceiveBuffer.data(), receiveBufferSize, 0)) < 0) {
            perror("client size recv");
            return false;
        }
        if (recvLength == 0) {
            return false;
        }
        logger.log(Logger::LogLevel::DEBUG, "Received [", recvLength, "/", msgLen, "]");
        received += recvLength;
        receiveBufferSize = BUFSIZ < (msgLen - received) ? BUFSIZ : (msgLen - received);
    }
    return true;
}

bool Server::setNonBlockingSocket(const int socket) {
    int flags = fcntl(socket, F_GETFL, 0);                  // get current flag
    return fcntl(socket, F_SETFL, flags | O_NONBLOCK) >= 0; // set non blocking flag;
}

TRANVANH_NAMESPACE_END
