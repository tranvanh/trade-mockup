#include "Toybox/Application.h"
#include "Toybox/Logger.h"

TOYBOX_NAMESPACE_BEGIN

Application::~Application() {
    isRunning = false;
    mThreadPool.stop();
}

void Application::run() {
    isRunning = true;
    mThreadPool.run();
}

void Application::stop() {
    isRunning = false;
    mThreadPool.stop();
}

void Application::runBackgroundTask(std::function<void()> f) {
    mThreadPool.addTask(std::move(f));
}

TOYBOX_NAMESPACE_END
