#pragma once
#include <string>

struct SocketData{
    char buffer[BUFSIZ+1];
    int size = 0;
    SocketData() = default;
    SocketData(const SocketData&) = delete;
    SocketData(SocketData&& other){
        std::swap(buffer, other.buffer);
        size = other.size;
    }
};

class Client {
    int mSocket = -1;
public:
    Client() = default;
    Client(const Client&) = delete;
    ~Client();
    bool openSocket();
    bool connectToServer(const std::string& url);

    // Receive from socket and returns buffer with number of bytes recieved
    // Does not guarantee the message is complete, there could be some left to receive. It is up to the caller to handle
    SocketData receive()const;    
};