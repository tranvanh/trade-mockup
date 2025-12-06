#include "ClientApplication.h"
#include <iostream>
#include <nlohmann/json.hpp>
#include <unordered_map>
#include <UtilsLib/Logger.h>

void ClientApplication::run() {
    Application::run();
    if(!mClient.openSocket() || !mClient.connectToServer("127.0.0.1", 8080)){
        stop();
        return;
    }
    if (mSimulation) {
        mGenerator.simulateMarket();
        while (isRunning) {
        }
    } else {
        processUserInputs();
    }
}

void ClientApplication::processUserInputs() const {
    std::string line;
    while (std::cout << "> " && std::getline(std::cin, line)) {
        if (line.empty())
            continue;
        Command cmd = parseCommand(line);
        switch(cmd.type){
            case CommandType::INVALID:
                Logger::instance().log(Logger::LogLevel::ERROR, "Invalid command");
                break;
            case CommandType::EXIT:
                return;
            default:
                handleCommand(cmd);
                break;
        }
    }
}

void ClientApplication::handleCommand(const Command& cmd) const {
    ASSERT(cmd.type == CommandType::BUY || cmd.type == CommandType::SELL, "Command behaviour not defined");
    OrderType type = cmd.type == CommandType::BUY ? OrderType::BUY : OrderType::SELL;
    Order     order(mId, type, cmd.price, cmd.volume);
    registerOrder(std::move(order));
}

ClientApplication::Command ClientApplication::parseCommand(const std::string& line) const {
    Command            cmd;
    std::istringstream iss(line);
    std::string        symbol;
    if (!(iss >> symbol)) {
        return cmd;
    }
    int    volume;
    double price;
    if (symbol == "buy") {
        if (iss >> price >> volume && price > 0 && volume > 0) {
            cmd.type   = CommandType::BUY;
            cmd.price  = price;
            cmd.volume = volume;
        }
    } else if (symbol == "sell") {
        if (iss >> price >> volume && price > 0 && volume > 0) {
            cmd.type   = CommandType::SELL;
            cmd.volume = volume;
            cmd.price  = price;
        }
    } else if (symbol == "exit") {
        cmd.type = CommandType::EXIT;
    }
    return cmd;
}

void ClientApplication::registerOrder(Order order) const {
    nlohmann::json msgJson;
    msgJson["clientId"]     = order.clientId;
    msgJson["type"]   = int(order.type);
    msgJson["price"]  = order.price;
    msgJson["volume"] = order.volume;
    const std::string msg   = nlohmann::to_string(msgJson);
    mClient.sendMessage(msg.c_str());
}