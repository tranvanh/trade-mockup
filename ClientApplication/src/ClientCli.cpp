#include "ClientCli.h"
#include "ClientApplication.h"
#include <TradeGUI/Ansi.h>
#include <TradeCore/Order.h>
#include <iostream>
#include <sstream>

// ── Cli ───────────────────────────────────────────────────────────────────────
ClientCli::ClientCli(ClientApplication& app) : TradeGUI::CliBase(), mApp(app) {}

// ── Mode selection ────────────────────────────────────────────────────────────
ClientCli::Mode ClientCli::selectMode() {
    std::cout << "\n  " << ansi::bold << "Select mode" << ansi::reset << "\n"
              << ansi::dim << "  ──────────────────────\n" << ansi::reset
              << "   " << ansi::bold << "1" << ansi::reset << "  Enter orders\n"
              << "   " << ansi::bold << "2" << ansi::reset << "  Simulate market\n";

    while (true) {
        const char* raw = mRx.input("\n  > ");
        if (!raw) return Mode::ORDER_ENTRY;
        mRx.print("\033[A\033[2K\r");
        std::string line(raw);
        if (line == "1") return Mode::ORDER_ENTRY;
        if (line == "2") return Mode::SIMULATE;
        std::cout << "  " << ansi::red << "Enter 1 or 2." << ansi::reset << "\n";
    }
}

// ── Order REPL ────────────────────────────────────────────────────────────────
static std::optional<TradeCore::Order> parseLine(const std::string& line, int clientId) {
    std::istringstream iss(line);
    std::string        cmd;
    if (!(iss >> cmd)) return std::nullopt;

    if (cmd == "buy" || cmd == "sell") {
        double price  = 0;
        int    volume = 0;
        if (iss >> price >> volume && price > 0 && volume > 0) {
            TradeCore::OrderType type = (cmd == "buy") ? TradeCore::OrderType::BUY
                                                       : TradeCore::OrderType::SELL;
            return TradeCore::Order(clientId, type, static_cast<int>(price), volume);
        }
        std::cout << "  " << ansi::red
                  << "Usage: " << cmd << " <price> <volume>"
                  << ansi::reset << "\n";
        return std::nullopt;
    }
    std::cout << "  " << ansi::red << "Unknown command." << ansi::reset
              << ansi::dim << "  buy · sell · exit\n" << ansi::reset;
    return std::nullopt;
}

void ClientCli::runOrderRepl() {
    std::cout << "\n"
              << ansi::dim
              << "  buy <price> <volume>  ·  sell <price> <volume>  ·  exit\n"
              << "  ─────────────────────────────────────────────────────────\n"
              << ansi::reset << "\n";

    mRx.set_completion_callback([](const std::string& prefix, int /*ctx*/) {
        replxx::Replxx::completions_t completions;
        for (const char* cmd : {"buy ", "sell ", "exit"})
            if (std::string(cmd).rfind(prefix, 0) == 0)
                completions.emplace_back(cmd);
        return completions;
    });

    const std::string replPrompt = "  trade > ";

    while (true) {
        const char* raw = mRx.input(replPrompt);
        if (!raw) break;
        mRx.print("\033[A\033[2K\r");
        std::string line(raw);
        if (line.empty()) continue;
        mRx.history_add(line);

        if (line == "exit" || line == "quit") break;

        auto order = parseLine(line, mApp.id());
        if (order) {
            const char* typeStr = (order->type == TradeCore::OrderType::BUY) ? "BUY" : "SELL";
            const int   vol     = order->volume;
            const int   price   = order->price;
            try {
                mApp.registerOrder(std::move(*order));
                std::cout << "  " << ansi::green << "✓" << ansi::reset
                          << "  Sent " << ansi::bold << typeStr << ansi::reset
                          << "  " << vol << " @ " << price << "\n\n";
            } catch (const std::exception& e) {
                std::cout << "  " << ansi::red << "✗  Failed to send: "
                          << e.what() << ansi::reset << "\n\n";
            }
        }
    }

    mRx.set_completion_callback(nullptr);
}

// ── Entry point ───────────────────────────────────────────────────────────────
void ClientCli::run() {
    printBanner("Client");

    const int         id     = inputInt("Client ID");
    const std::string server = inputString("Server", "127.0.0.1");
    const int         port   = inputInt("Port", 8080);

    mApp.configure(id, server, port);

    std::cout << "\n  Connecting to "
              << ansi::cyan << server << ":" << port << ansi::reset << " ...";
    std::cout.flush();

    try {
        if (!mApp.connect()) {
            std::cout << "\n  " << ansi::red << "✗  Connection refused\n" << ansi::reset;
            return;
        }
        std::cout << "  " << ansi::green << "✓  Connected\n" << ansi::reset;

        switch (selectMode()) {
            case Mode::ORDER_ENTRY: runOrderRepl(); break;
            case Mode::SIMULATE:
                mApp.simulateMarket();
                std::cout << "\n  " << ansi::green << "Simulation running." << ansi::reset
                          << ansi::dim << "  Type 'quit' or press Ctrl-D to stop.\n" << ansi::reset;
                while (true) {
                    const char* raw = mRx.input("  simulating > ");
                    if (!raw) break;
                    mRx.print("\033[A\033[2K\r");
                    std::string line(raw);
                    if (line == "quit" || line == "exit") break;
                }
                break;
        }
    } catch (const std::exception& e) {
        std::cout << "\n  " << ansi::red << "✗  " << e.what() << ansi::reset << "\n";
    }
}
