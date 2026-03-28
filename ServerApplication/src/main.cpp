#include "ServerApplication.h"
#include <Toybox/Logger.h>

int main() {
    toybox::Logger::instance().setLevel(toybox::Logger::LogLevel::ERROR);
    ServerApplication app;
    app.run();
    return 0;
}
