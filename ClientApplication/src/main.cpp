#include "ClientApplication.h"
#include "ClientUI.h"
#include <Toybox/Logger.h>

int main() {
    toybox::Logger::instance().setLevel(toybox::Logger::LogLevel::DEBUG);
    ClientApplication app;
    app.run();
    ClientUI ui(app);
    ui.run();
    app.stop();
    return 0;
}
