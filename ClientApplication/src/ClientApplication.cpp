#include "ClientApplication.h"
#include <nlohmann/json.hpp>
#include <Toybox/Logger.h>
#include <stdexcept>

ClientApplication::ClientApplication()
    : toybox::Application(THREAD_COUNT)
    , mGenerator(*this)
    , mCli(*this) {}

void ClientApplication::run() {
    mCli.run();
}

void ClientApplication::configure(int id, std::string server, int port) {
    mId     = static_cast<uint>(id);
    mServer = std::move(server);
    mPort   = port;
}

bool ClientApplication::connect() {
    try {
        Application::run();
        if (!mClient.connect(mServer, mPort))
            return false;
        mClient.run();
        return true;
    } catch (const std::exception& e) {
        stop();
        throw std::runtime_error(
            std::string("Connection to ") + mServer + ":" + std::to_string(mPort)
            + " failed: " + e.what());
    }
}

void ClientApplication::simulateMarket() {
    runBackgroundTask([this]() { mGenerator.simulateMarket(); });
}

void ClientApplication::registerOrder(TradeCore::Order order) const {
    nlohmann::json msg;
    msg["clientId"] = order.clientId;
    msg["type"]     = int(order.type);
    msg["price"]    = order.price;
    msg["volume"]   = order.volume;
    mClient.sendMessage(nlohmann::to_string(msg));
}
