#pragma once
#include <string>
#include <vector>

namespace TradeCommon {

    struct ServerLaunchOptions {
        bool filled = false;
    };

    struct ServerParseResult {
        ServerLaunchOptions options;
        bool                showHelp = false;
        std::string         helpText;
    };

    ServerParseResult parseServerLaunchOptions(int argc, char* argv[]);
    ServerParseResult parseServerLaunchOptions(const std::vector<std::string>& args);
}
