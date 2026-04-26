#pragma once
#include <deque>
#include <mutex>
#include <string>
#include <vector>

namespace tradegui {
    constexpr std::size_t MAX_LOG_LINES = 10;
    constexpr std::size_t MAX_TRADE_LINES = 10;
    constexpr std::size_t MAX_CMD_HISTORY = 10;

    struct UIState {
        struct {
            mutable std::mutex mtx;
            std::deque<std::string> data;
            std::size_t totalTrades = 0;
        } trades;

        struct {
            mutable std::mutex mtx;
            std::deque<std::string> data;
        } logs;

        struct {
            mutable std::mutex mtx;
            std::deque<std::string> data;
        } cmdHistory;

        struct {
            mutable std::mutex mtx;
            std::vector<unsigned short> data;
        } connections;

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
