#pragma once
#include <string>
#include <functional>
#include "UtilsLib/NetworkUtils.h"

class Server : public NetworkComponent {
public:
    enum class AddressType{
        ANY,
        LOCAL,
        PUBLIC,
    };
    AddressType mAddressType = AddressType::ANY;
    std::string mAddress;
    Server(const AddressType addressType, const std::string& address = "");
    Server(const Server&) = delete;
    // virtual ~Server() = default;
    bool startListen(const int port, const std::function<void(const SocketData)>& onReceive);
};