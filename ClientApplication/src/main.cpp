#include "UtilsLib/Client.h"
#include "UtilsLib/Logger.h"
#include <iostream>
#include <netinet/in.h>
#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>
#include <unordered_map>

// class Test : public Client {
// public:
//     void sendMessage() {
//         const char* req = "Hello server";
//         send(getSocket(), req, strlen(req), 0);
//         Logger::instance().log(Logger::LogLevel::INFO, "msg sent");
//     }
// };

int main() {
    auto& logger = Logger::instance();
    Client  client;
    client.openSocket();
    client.connectToServer("127.0.0.1", 8080);

    enum Command { SEND, EXIT };
    const std::unordered_map<std::string, Command> commands = {
        { "send", SEND },
        { "exit", EXIT },
    };

    std::string command = "";
    while (std::cin >> command) {
        logger.log(Logger::LogLevel::INFO, "Command: ", command);
        // std::transform(command.begin(), command.end(), command.begin(), ::tolower);
        const auto commandIter = commands.find(command);
        if (commandIter == commands.end()) {
            logger.log(Logger::LogLevel::INFO, "Command not found");
            continue;
        }
        switch (commandIter->second) {
        case Command::SEND: {
            std::string msg;
            std::cin >> msg;
            client.sendMessage(msg.c_str());
            break;
        }
        case Command::EXIT:
            return 0;
        default:
            logger.log(Logger::LogLevel::INFO, "Command behaviour not defined");
            break;
        }
    }
    return 0;
}