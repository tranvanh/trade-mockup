#include "ClientApplication.h"
#include <UtilsLib/Logger.h>
#include <thread>

int main(int argc, char * argv[]) {
    auto& logger = Logger::instance();
    if(argc == 0){
         logger.log(Logger::LogLevel::ERROR, "No id number specified");
         return 1;
    }

    int id = strtol(argv[1], nullptr, 0);
    if(id < 0){
        logger.log(Logger::LogLevel::ERROR, "Invalid id");
        return 1;
    }
    bool simulate = false;
    if(argc > 2){
        std::string argument(argv[2]);
        if(argument == "simulate"){
            logger.log(Logger::LogLevel::INFO, "Order simulation enabled");
            logger.log(Logger::LogLevel::INFO, "Simulation will start shortly...");
            simulate = true;
            // Time for user to register the log
            std::this_thread::sleep_for(std::chrono::duration<int, std::milli>(2000));
        }
        else{
            logger.log(Logger::LogLevel::WARNING, "Invalid is argument and it will be ignored...");
        }
    }

    ClientApplication app(id, simulate);
    app.run();
    return 0;
}