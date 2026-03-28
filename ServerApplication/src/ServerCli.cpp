#include "ServerCli.h"
#include "ServerApplication.h"
#include <TradeGUI/Ansi.h>
#include <TradeCore/Trade.h>
#include <chrono>
#include <ctime>
#include <iostream>
#include <sstream>

static std::string timestamp() {
    auto now = std::chrono::system_clock::now();
    std::time_t t = std::chrono::system_clock::to_time_t(now);
    std::tm tm{};
    localtime_r(&t, &tm);
    char buf[9];
    std::strftime(buf, sizeof(buf), "%H:%M:%S", &tm);
    return buf;
}

static std::string formatTrade(const TradeCore::Trade& trade) {
    std::ostringstream oss;
    oss << ansi::gray  << timestamp() << ansi::reset << "  "
        << ansi::green << ansi::bold  << "TRADE" << ansi::reset << "  "
        << ansi::dim   << "buyer:" << ansi::reset << ansi::bold << trade.buyerId  << ansi::reset
        << "  "
        << ansi::dim   << "seller:" << ansi::reset << ansi::bold << trade.sellerId << ansi::reset
        << "  "
        << ansi::yellow << trade.volume << " @ " << trade.price << ansi::reset;
    return oss.str();
}

static std::string formatEvent(const std::string& msg) {
    std::ostringstream oss;
    oss << ansi::gray << timestamp() << ansi::reset << "  "
        << ansi::cyan << ansi::bold << "EVENT" << ansi::reset << "  "
        << msg;
    return oss.str();
}

// ── ServerCli ─────────────────────────────────────────────────────────────────
ServerCli::ServerCli(ServerApplication& app) : TradeGUI::CliBase(), mApp(app) {
    mRx.set_completion_callback([](const std::string& prefix, int) {
        replxx::Replxx::completions_t out;
        for (const char* cmd : {"events", "trades", "help", "quit"})
            if (std::string(cmd).rfind(prefix, 0) == 0)
                out.emplace_back(cmd);
        return out;
    });
}

// ── Setup prompts ─────────────────────────────────────────────────────────────
// Port uses "accept bad input, fall back to 8080" semantics rather than the
// base-class inputInt retry loop, so the test flow (send bad → see Prefill) works.
void ServerCli::promptConfig(int& port, bool& filled) {
    std::cout << "\n  " << ansi::bold << "Port" << ansi::reset
              << ansi::dim << "  (default: 8080)" << ansi::reset << "\n";
    mRx.set_preload_buffer("8080");
    const char* raw = mRx.input("  > ");
    std::string portStr = (raw && *raw) ? raw : "8080";
    try {
        std::size_t pos;
        port = std::stoi(portStr, &pos);
        if (pos != portStr.size()) throw std::invalid_argument("");
    } catch (...) {
        std::cout << "  " << ansi::red << "Invalid port, using 8080." << ansi::reset << "\n";
        port = 8080;
    }
    if (raw) mRx.history_add(portStr);

    std::string answer = inputString("Prefill market with mock data? [y/N]", std::string("N"));
    filled = (answer == "y" || answer == "Y");
}

// ── View header ───────────────────────────────────────────────────────────────
void ServerCli::printViewHeader(View view) {
    const bool eventsActive = (view == View::EVENTS);
    const bool tradesActive = (view == View::TRADES);

    std::string header;
    header += "  ";
    header += eventsActive ? (std::string(ansi::bold) + ansi::cyan + "[ 1  Events ]" + ansi::reset)
                           : (std::string(ansi::dim)  + "[ 1  Events ]" + ansi::reset);
    header += "   ";
    header += tradesActive ? (std::string(ansi::bold) + ansi::cyan + "[ 2  Trades ]" + ansi::reset)
                           : (std::string(ansi::dim)  + "[ 2  Trades ]" + ansi::reset);
    header += ansi::dim;
    header += std::string("   ─────   commands: help") + ansi::reset + "\n";
    header += ansi::dim;
    header += std::string(60, '-') + ansi::reset + "\n";

    mRx.print(header.c_str());
}

// ── Queue draining (CLI thread only) ─────────────────────────────────────────
void ServerCli::drainQueues() {
    while (auto item = mEventQueue.try_pop()) {
        mEventHistory.push_back(std::move(*item));
        if (static_cast<int>(mEventHistory.size()) > MAX_LOG)
            mEventHistory.erase(mEventHistory.begin());
    }
    while (auto item = mTradeQueue.try_pop()) {
        mTradeHistory.push_back(std::move(*item));
        if (static_cast<int>(mTradeHistory.size()) > MAX_LOG)
            mTradeHistory.erase(mTradeHistory.begin());
    }
}

// ── View switching ────────────────────────────────────────────────────────────
void ServerCli::switchView(View view) {
    mCurrentView = view;
    drainQueues();

    mRx.print("\033[2J\033[H");
    printViewHeader(view);

    const auto& history = (view == View::EVENTS) ? mEventHistory : mTradeHistory;
    if (history.empty()) {
        mRx.print(
            (std::string(ansi::dim) + "  (no entries yet)\n" + ansi::reset).c_str());
    } else {
        for (const auto& line : history)
            mRx.print((line + "\n").c_str());
    }
    mRx.print((std::string(ansi::dim) + std::string(60, '-') + ansi::reset + "\n").c_str());
}

// ── Thread-safe log append (called from background threads) ──────────────────
void ServerCli::appendEvent(std::string msg) {
    std::string line = formatEvent(msg);
    mEventQueue.push(line);
    if (mCurrentView == View::EVENTS)
        mRx.print((line + "\n").c_str());
}

void ServerCli::appendTrade(const TradeCore::Trade& trade) {
    std::string line = formatTrade(trade);
    mTradeQueue.push(line);
    if (mCurrentView == View::TRADES)
        mRx.print((line + "\n").c_str());
}

// ── Command handling ──────────────────────────────────────────────────────────
std::string ServerCli::currentPrompt() const {
    const char* view = (mCurrentView == View::EVENTS) ? "events" : "trades";
    return std::string("  ") + view + " > ";
}

void ServerCli::handleCommand(const std::string& cmd) {
    if (cmd == "1" || cmd == "events") {
        switchView(View::EVENTS);
    } else if (cmd == "2" || cmd == "trades") {
        switchView(View::TRADES);
    } else if (cmd == "help") {
        mRx.print(
            (std::string(ansi::bold) + "  Commands:\n" + ansi::reset +
             ansi::dim +
             "    events / 1   — switch to events view\n"
             "    trades / 2   — switch to trades view\n"
             "    help         — show this message\n"
             "    quit / exit  — stop the server\n"
             + ansi::reset).c_str());
    } else {
        mRx.print(
            (std::string(ansi::red) + "  Unknown command: " + cmd + ansi::reset +
             ansi::dim + "  (type 'help')\n" + ansi::reset).c_str());
    }
}

// ── Entry point ───────────────────────────────────────────────────────────────
void ServerCli::run() {
    printBanner("Server");

    int  port   = 8080;
    bool filled = false;
    promptConfig(port, filled);

    std::cout << "\n  Starting server on port "
              << ansi::cyan << port << ansi::reset << " ...";
    std::cout.flush();

    try {
        mApp.startBackground(port, filled);

        std::cout << "  " << ansi::green << "OK  Running\n\n" << ansi::reset;

        switchView(View::EVENTS);

        while (true) {
            const char* raw = mRx.input(currentPrompt());
            if (!raw) break;               // Ctrl-D / EOF
            std::string cmd(raw);
            if (cmd.empty()) continue;
            mRx.history_add(cmd);

            if (cmd == "quit" || cmd == "exit") break;
            handleCommand(cmd);
        }

        mApp.stop();

    } catch (const std::exception& e) {
        std::cout << "\n  " << ansi::red << "Failed: " << e.what() << ansi::reset << "\n";
    }
}
