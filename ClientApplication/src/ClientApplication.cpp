#include "ClientApplication.h"
#include <iostream>
#include <nlohmann/json.hpp>
#include <unordered_map>
#include <UtilsLib/Logger.h>

void ClientApplication::run() {
    Application::run();
    isRunning = true;
    mClient.openSocket();
    // add assert checking the socket
    mClient.connectToServer("127.0.0.1", 8080);
    if (mSimulation) {
        mGenerator.simulateMarket();
        while(isRunning){}
    } else {
        processUserInputs();
    }
}

void ClientApplication::processUserInputs() const {
    auto&       logger  = Logger::instance();
    std::string command = "";
    while (std::cin >> command) {
        logger.log(Logger::LogLevel::DEBUG, "Command: ", command);
        const auto commandIter = mCommands.find(command);
        if (commandIter == mCommands.end()) {
            logger.log(Logger::LogLevel::INFO, "Command not found");
            continue;
        }
        Order order;
        order.id = mId;
        switch (commandIter->second) {
        case Command::SELL: {
            order.type = OrderType::SELL;
            proccessOrderInput(order);
            registerOrder(std::move(order));
            break;
        }
        case Command::BUY: {
            order.type = OrderType::BUY;
            proccessOrderInput(order);
            registerOrder(std::move(order));
            break;
        }
        case Command::EXIT:
            return;
        default:
            logger.log(Logger::LogLevel::INFO, "Command behaviour not defined");
            break;
        }
        // Clear the stream from invalid trailing text
        // std::cin.clear();
        // std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    }
}

void ClientApplication::proccessOrderInput(Order& order) const {

    // \todo must me some validation
    std::cin >> order.price;
    std::cin >> order.volume;
}

std::string escape_quotes(const std::string& input) {
    std::string output;
    output.reserve(input.size() * 2); // reserve extra space

    for (char c : input) {
        if (c == '"') {
            output.push_back('\\'); // add escape
        }
        output.push_back(c);
    }
    return output;
}

void ClientApplication::registerOrder(Order order) const {
    nlohmann::json msgJson;
    msgJson["id"]     = order.id;
    msgJson["type"]   = int(order.type);
    msgJson["price"]  = order.price;
    msgJson["volume"] = order.volume;
    std::string msg   = nlohmann::to_string(msgJson);
    mClient.sendMessage(msg.c_str());
}