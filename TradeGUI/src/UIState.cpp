#include "TradeGUI/UIState.h"

namespace tradegui {

void UIState::pushTrade(std::string line) {
    std::lock_guard lock(trades.mtx);
    if (trades.data.size() >= MAX_TRADE_LINES)
        trades.data.pop_front();
    trades.data.push_back(std::move(line));
    ++trades.totalTrades;
}

void UIState::pushLog(std::string line) {
    std::lock_guard lock(logs.mtx);
    if (logs.data.size() >= MAX_LOG_LINES)
        logs.data.pop_front();
    logs.data.push_back(std::move(line));
}

void UIState::pushCmd(std::string line) {
    std::lock_guard lock(cmdHistory.mtx);
    if (cmdHistory.data.size() >= MAX_CMD_HISTORY)
        cmdHistory.data.pop_front();
    cmdHistory.data.push_back(std::move(line));
}

void UIState::addConnection(unsigned short port) {
    std::lock_guard lock(connections.mtx);
    connections.data.push_back(port);
}

void UIState::removeConnection(unsigned short port) {
    std::lock_guard lock(connections.mtx);
    auto it = std::find(connections.data.begin(), connections.data.end(), port);
    if (it != connections.data.end())
        connections.data.erase(it);
}

std::deque<std::string> UIState::snapshotTrades() const {
    std::lock_guard lock(trades.mtx);
    return trades.data;
}

std::deque<std::string> UIState::snapshotLogs() const {
    std::lock_guard lock(logs.mtx);
    return logs.data;
}

std::deque<std::string> UIState::snapshotCmds() const {
    std::lock_guard lock(cmdHistory.mtx);
    return cmdHistory.data;
}

std::vector<unsigned short> UIState::snapshotConnections() const {
    std::lock_guard lock(connections.mtx);
    return connections.data;
}

std::size_t UIState::snapshotTotalTrades() const {
    std::lock_guard lock(trades.mtx);
    return trades.totalTrades;
}

} // namespace tradegui
