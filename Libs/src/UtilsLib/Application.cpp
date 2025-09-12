#include "Application.h"

Application::~Application() {
    isRunning = false;
    for (auto& t : mThreadPool) {
        t.join();
    }
}

void Application::runBackgroundTask(std::function<void()> f) {
    mThreadPool.emplace_front(std::move(f));
}