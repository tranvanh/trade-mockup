#include "UtilsLib/Client.h"
#include <iostream>

int main() {
    // Logger::instance().log(Logger::LogLevel::INFO, "TEST");
    Client client;
    client.openSocket();
    client.connectToServer("time.nist.gov");
    SocketData data = client.receive();
    if (data.size < 0) {
        return 1;
    }
    std::string test(data.buffer, data.size);
    std::cout << test << std::endl;


    return 0;
}