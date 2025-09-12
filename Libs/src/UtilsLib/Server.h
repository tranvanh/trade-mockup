#pragma once
#include <string>
#include <functional>
#include <vector>
#include "UtilsLib/NetworkUtils.h"

class Server : public NetworkComponent {
public:
    enum class AddressType {
        ANY,
        LOCAL,
        PUBLIC,
    };
    AddressType mAddressType = AddressType::ANY;
    std::string mAddress;
    Server(const AddressType addressType, const std::string& address = "");
    Server(const Server&) = delete;
    // virtual ~Server() = default;
    bool startListen(const int port, std::function<void(std::vector<char>, const int)> onReceive);

private:
    bool receive(const int clientSocket, std::function<void(std::vector<char>, const int)> onReceive) const;
    bool receiveSize(const int clientSocket, size_t& out) const;
    bool receiveContent(const int clientSocket, const size_t msgLen, std::vector<char>& out) const;
};