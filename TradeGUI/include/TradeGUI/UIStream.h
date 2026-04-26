#pragma once
#include <functional>
#include <ostream>
#include <streambuf>
#include <string>

namespace tradegui {

class UIStreamBuf : public std::streambuf {
    std::string mLine;
    std::function<void(std::string)> mOnLine;

protected:
    int overflow(int c) override;
    std::streamsize xsputn(const char* s, std::streamsize n) override;

public:
    explicit UIStreamBuf(std::function<void(std::string)> onLine);
};

class UIStream : public std::ostream {
    UIStreamBuf mBuf;

public:
    explicit UIStream(std::function<void(std::string)> onLine);
};

} // namespace tradegui
