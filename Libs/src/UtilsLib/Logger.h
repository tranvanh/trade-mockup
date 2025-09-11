#pragma once
#include "UtilsLib/TimePointUtils.h"
#include <chrono>
#include <iostream>
#include <mutex>
#include <sstream>

class Logger {
public:
    enum class LogLevel { DEBUG, INFO, WARNING, ERROR };

private:
    Logger()  = default;
    ~Logger() = default;

    struct {
        std::ostream* stream = &std::cout;
        std::mutex    lock;
    } mOutput;
    LogLevel mLogLevel = LogLevel::DEBUG;

public:
    static Logger& instance();
    void setOutputStream(std::ostream& out);
    void setLevel(LogLevel logLevel) { mLogLevel = logLevel; }
    template <typename... Args>
    void log(LogLevel logLevel, Args&&... args) {
        if (logLevel < mLogLevel) {
            return;
        }
        std::ostringstream stream;
        (stream << ... << args); // Fold expression (C++17)

        auto                        now = std::chrono::system_clock::now();
        std::lock_guard<std::mutex> lock(mOutput.lock);
        *mOutput.stream << now << " [" << levelToString(logLevel) << "] " << stream.str() << std::endl;
    }

private:
    std::string levelToString(LogLevel level);
};