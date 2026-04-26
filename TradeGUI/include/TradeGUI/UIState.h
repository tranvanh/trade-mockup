#pragma once
#include <deque>
#include <mutex>
#include <string>
#include <vector>

namespace tradegui {

constexpr std::size_t MAX_LOG_LINES   = 300;
constexpr std::size_t MAX_TRADE_LINES = 300;
constexpr std::size_t MAX_CMD_HISTORY = 100;

struct UIState {
    mutable std::mutex mtx;
    std::deque<std::string> trades;
    std::deque<std::string> logs;
    std::deque<std::string> cmdHistory;
    std::vector<unsigned short> connections;
    std::size_t totalTrades = 0;

    void pushTrade(std::string line);
    void pushLog(std::string line);
    void pushCmd(std::string line);
    void addConnection(unsigned short port);
    void removeConnection(unsigned short port);

    std::deque<std::string> snapshotTrades() const;
    std::deque<std::string> snapshotLogs() const;
    std::deque<std::string> snapshotCmds() const;
    std::vector<unsigned short> snapshotConnections() const;
    std::size_t snapshotTotalTrades() const;
};

} // namespace tradegui
