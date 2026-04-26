#include "TradeGUI/UIState.h"
#include <algorithm>

namespace tradegui {

void UIState::pushTrade(std::string line) {
    std::lock_guard lock(mtx);
    if (trades.size() >= MAX_TRADE_LINES)
        trades.pop_front();
    trades.push_back(std::move(line));
    ++totalTrades;
}

void UIState::pushLog(std::string line) {
    std::lock_guard lock(mtx);
    if (logs.size() >= MAX_LOG_LINES)
        logs.pop_front();
    logs.push_back(std::move(line));
}

void UIState::pushCmd(std::string line) {
    std::lock_guard lock(mtx);
    if (cmdHistory.size() >= MAX_CMD_HISTORY)
        cmdHistory.pop_front();
    cmdHistory.push_back(std::move(line));
}

void UIState::addConnection(unsigned short port) {
    std::lock_guard lock(mtx);
    connections.push_back(port);
}

void UIState::removeConnection(unsigned short port) {
    std::lock_guard lock(mtx);
    auto it = std::find(connections.begin(), connections.end(), port);
    if (it != connections.end())
        connections.erase(it);
}

std::deque<std::string> UIState::snapshotTrades() const {
    std::lock_guard lock(mtx);
    return trades;
}

std::deque<std::string> UIState::snapshotLogs() const {
    std::lock_guard lock(mtx);
    return logs;
}

std::deque<std::string> UIState::snapshotCmds() const {
    std::lock_guard lock(mtx);
    return cmdHistory;
}

std::vector<unsigned short> UIState::snapshotConnections() const {
    std::lock_guard lock(mtx);
    return connections;
}

std::size_t UIState::snapshotTotalTrades() const {
    std::lock_guard lock(mtx);
    return totalTrades;
}

} // namespace tradegui
