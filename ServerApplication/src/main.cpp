#include "ServerApplication.h"
#include "ServerUI.h"
#include <Toybox/Logger.h>

int main() {
    toybox::Logger::instance().setLevel(toybox::Logger::LogLevel::DEBUG);
    ServerApplication app;
    app.run();
    ServerUI ui(app);
    ui.run();
    app.stop();
    return 0;
}
