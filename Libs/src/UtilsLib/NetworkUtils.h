#pragma once
#include <memory>

// struct SocketData{
//     char buffer[BUFSIZ+1];
//     int size = 0;
//     SocketData() = default;
//     SocketData(const SocketData&other) { std::memcpy(buffer, other.buffer, size);};
//     SocketData(SocketData&& other){
//         std::swap(buffer, other.buffer);
//         size = other.size;
//     }
// };

class NetworkComponent {
protected:
    int mSocket = -1;
    NetworkComponent()                        = default;
    NetworkComponent(const NetworkComponent&) = delete;
    virtual ~NetworkComponent();

public:
    int  getSocket() const;
    bool openSocket();
};
