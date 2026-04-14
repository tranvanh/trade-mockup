#include "ClientApplication.h"
#include <TradeCommon/ClientOptions.h>
#include <Toybox/Logger.h>

int main(int argc, char* argv[]) {
    toybox::Logger::instance().setLevel(toybox::Logger::LogLevel::ERROR);
    try {
        const auto parsed = TradeCommon::parseClientLaunchOptions(argc, argv);
        if (parsed.showHelp) {
            std::cout << parsed.helpText << std::endl;
            return 0;
        }
        ClientApplication app(parsed.options);
        app.run();
    } catch (const std::exception& e) {
        std::cerr << "Client startup error: " << e.what() << "\n";
        return 1;
    }
    return 0;
}
