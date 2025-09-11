#pragma once
#include "UtilsLib/NetworkUtils.h"

class Client : public NetworkComponent {
public:
    virtual ~Client() = default;
    bool connectToServer(const char* url, const int port) const;
    
    // \todo currently unused, but be refractored
    // Receive from socket and returns buffer with number of bytes recieved
    // Does not guarantee the message is complete, there could be some left to receive. It is up to the caller
    // to handle
    // SocketData receive() const;
    bool sendMessage(const char* msg) const;
};