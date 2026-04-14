#include "ServerApplication.h"
#include <TradeCommon/ServerOptions.h>
#include <Toybox/Logger.h>

int main(int argc, char* argv[]) {
    toybox::Logger::instance().setLevel(toybox::Logger::LogLevel::ERROR);
    try {
        const auto parsed = TradeCommon::parseServerLaunchOptions(argc, argv);
        if (parsed.showHelp) {
            std::cout << parsed.helpText << std::endl;
            return 0;
        }
        ServerApplication app(parsed.options);
        app.run();
        return 0;
    } catch (const std::exception& e) {
        std::cerr << "Server startup error: " << e.what() << "\n";
        return 1;
    }
}
