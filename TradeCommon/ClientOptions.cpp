#include "TradeCommon/ClientOptions.h"
#include <cxxopts.hpp>
#include <stdexcept>

namespace TradeCommon {
    namespace {
        cxxopts::Options makeOptions() {
            cxxopts::Options options("ClientApplication", "Trading client");
            options.add_options()("id", "Client id for legacy manual mode", cxxopts::value<int>())(
                "simulate",
                "Run legacy simulation mode",
                cxxopts::value<bool>()->implicit_value("true"))("h,help", "Print help");
            return options;
        }

        ClientParseResult buildResult(cxxopts::Options& options, const cxxopts::ParseResult& result) {
            ClientParseResult parsed;
            parsed.helpText = options.help();
            if (result.count("help")) {
                parsed.showHelp = true;
                return parsed;
            }

            const bool hasId      = result.count("id") > 0;
            const bool simulate   = result.count("simulate") > 0 && result["simulate"].as<bool>();

            if (hasId && simulate) {
                throw std::invalid_argument("Cannot combine --id with --simulate");
            }

            if (hasId) {
                parsed.options.clientId = result["id"].as<int>();
                if (parsed.options.clientId <= 0) {
                    throw std::invalid_argument("Client id must be positive");
                }
                parsed.options.mode = ClientLaunchMode::LegacyManual;
                return parsed;
            }

            if (simulate) {
                parsed.options.mode = ClientLaunchMode::LegacySimulation;
            }

            return parsed;
        }
    }

    ClientParseResult parseClientLaunchOptions(int argc, char* argv[]) {
        auto             options = makeOptions();
        const auto       result  = options.parse(argc, argv);
        return buildResult(options, result);
    }

    ClientParseResult parseClientLaunchOptions(const std::vector<std::string>& args) {
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
