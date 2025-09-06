#include "UtilsLib/Logger.h"

// Logger& Logger::instance() {
//     static Logger logger;
//     return logger;
// }

void Logger::setOutputStream(std::ostream& out) {
    std::lock_guard<std::mutex> lock(mOutput.lock);
    mOutput.stream = &out;
}

std::string Logger::levelToString(LogLevel level) {
    switch (level) {
    case LogLevel::DEBUG:
        return "DEBUG";
    case LogLevel::INFO:
        return "INFO";
    case LogLevel::WARNING:
        return "WARNING";
    case LogLevel::ERROR:
        return "ERROR";
    }
    return "UNKNOWN";
}