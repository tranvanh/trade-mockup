#include "UtilsLib/Application.h"
#include "UtilsLib/Logger.h"

TRANVANH_NAMESPACE_BEGIN

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

TRANVANH_NAMESPACE_END
