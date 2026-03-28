#include "ClientApplication.h"
#include <Toybox/Logger.h>

int main() {
    toybox::Logger::instance().setLevel(toybox::Logger::LogLevel::ERROR);
    ClientApplication app;
    app.run();
    return 0;
}
