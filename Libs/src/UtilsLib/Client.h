#pragma once
#include "UtilsLib/NetworkUtils.h"
#include <string>

class Client : public NetworkComponent {
public:
    virtual ~Client() = default;
    bool connectToServer(const std::string& url);
    // Receive from socket and returns buffer with number of bytes recieved
    // Does not guarantee the message is complete, there could be some left to receive. It is up to the caller
    // to handle
    SocketData receive() const;
};