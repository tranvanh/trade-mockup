#include "ClientApplication.h"
#include <cxxopts.hpp>

int main(int argc, char* argv[]) {
    bool simulate = false;
    int  id       = 0;
    try {
        cxxopts::Options options("ClientApp", "Example argument parsing");
        options.add_options()("id", "client id", cxxopts::value<int>())("simulate",
                                                                        "generate orders",
                                                                        cxxopts::value<bool>())("h,help",
                                                                                                "Print help");
        const auto result = options.parse(argc, argv);
        if (result.count("help")) {
            std::cout << options.help() << std::endl;
            return 0;
        }
        if (result.count("id")) {
            id = result["id"].as<int>();
        } else if (result.count("simulate")) {
            simulate = result["simulate"].as<bool>();
        }
        else {
            std::cerr << "Error: --id or --simulate required\n";
            return 1;
        }
    } catch (const std::exception& e) {
        std::cerr << "Argument parsing error: " << e.what() << "\n";
        return 1;
    }
    ClientApplication app(id, simulate);
    app.run();
    return 0;
}