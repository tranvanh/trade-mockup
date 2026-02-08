#pragma once
#include "UtilsLib/NetworkComponent.h"
#include <functional>
#include <netinet/in.h>
#include <string>
#include <unordered_map>
#include <vector>

TRANVANH_NAMESPACE_BEGIN

/// Current implementation acts as a TCP receiver where we expect to read size of message first and then the
/// content later We poll events allowing high number of clients to be connected
class Server : public NetworkComponent {
    struct ClientConnection {
        enum class State {
            WAITING,
            SIZE_RECEIVED,
        };
        State  state  = State::WAITING;
        size_t msgLen = 0;
    };

    std::vector<char> mReceiveBuffer = std::vector<char>(BUFSIZ);

public:
    enum class AddressType {
        ANY,
        LOCAL,
        PUBLIC,
    };
    AddressType                               mAddressType = AddressType::ANY;
    std::string                               mAddress;
    std::unordered_map<int, ClientConnection> mClientConnections;

    Server(const AddressType addressType, const std::string& address = "");
    Server(const Server&) = delete;
    bool startListen(const int                                         socketId,
                     const int                                         port,
                     std::function<void(std::vector<char>, const int)> onReceive);

private:
    bool poll(const int                                         socketId,
              sockaddr_in                                       socketAddress,
              socklen_t                                         socketLen,
              std::function<void(std::vector<char>, const int)> onReceive);
    bool setNonBlockingSocket(const int socketId);

    // Server receiving follows a rule of receiving message size first and content after
    bool receive(const int clientSocket, std::function<void(std::vector<char>, const int)> onReceive);
    bool receiveSize(const int clientSocket, size_t& out);
    bool receiveContent(const int clientSocket, const size_t msgLen);
};

TRANVANH_NAMESPACE_END
