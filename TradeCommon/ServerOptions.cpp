#include "TradeCommon/ServerOptions.h"
#include <cxxopts.hpp>

namespace TradeCommon {
    namespace {
        cxxopts::Options makeOptions() {
            cxxopts::Options options("ServerApplication", "Trading server");
            options.add_options()("filled",
                                  "Pre-fill market with mock data",
                                  cxxopts::value<bool>()->implicit_value("true"))("h,help", "Print help");
            return options;
        }

        ServerParseResult buildResult(cxxopts::Options& options, const cxxopts::ParseResult& result) {
            ServerParseResult parsed;
            parsed.helpText = options.help();
            if (result.count("help")) {
                parsed.showHelp = true;
                return parsed;
            }

            parsed.options.filled = result.count("filled") > 0 && result["filled"].as<bool>();
            return parsed;
        }
    }

    ServerParseResult parseServerLaunchOptions(int argc, char* argv[]) {
        auto       options = makeOptions();
        const auto result  = options.parse(argc, argv);
        return buildResult(options, result);
    }

    ServerParseResult parseServerLaunchOptions(const std::vector<std::string>& args) {
        std::vector<char*> argv;
        argv.reserve(args.size());
        for (const auto& arg : args) {
            argv.push_back(const_cast<char*>(arg.c_str()));
        }

        auto       options = makeOptions();
        const auto result  = options.parse(static_cast<int>(argv.size()), argv.data());
        return buildResult(options, result);
    }
}
