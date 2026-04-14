#pragma once
#include <string>
#include <vector>

namespace TradeCommon {

    enum class ClientLaunchMode { Tui, LegacyManual, LegacySimulation };

    struct ClientLaunchOptions {
        ClientLaunchMode mode     = ClientLaunchMode::Tui;
        int              clientId = 0;
    };

    struct ClientParseResult {
        ClientLaunchOptions options;
        bool                showHelp = false;
        std::string         helpText;
    };

    ClientParseResult parseClientLaunchOptions(int argc, char* argv[]);
    ClientParseResult parseClientLaunchOptions(const std::vector<std::string>& args);
}
