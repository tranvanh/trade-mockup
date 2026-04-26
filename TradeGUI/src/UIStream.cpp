#include "TradeGUI/UIStream.h"

namespace tradegui {

UIStreamBuf::UIStreamBuf(std::function<void(std::string)> onLine)
    : mOnLine(std::move(onLine)) {}

int UIStreamBuf::overflow(int c) {
    if (c == EOF)
        return EOF;
    if (c == '\n') {
        if (mOnLine)
            mOnLine(mLine);
        mLine.clear();
    } else {
        mLine += static_cast<char>(c);
    }
    return c;
}

std::streamsize UIStreamBuf::xsputn(const char* s, std::streamsize n) {
    for (std::streamsize i = 0; i < n; ++i)
        overflow(static_cast<unsigned char>(s[i]));
    return n;
}

UIStream::UIStream(std::function<void(std::string)> onLine)
    : std::ostream(&mBuf)
    , mBuf(std::move(onLine)) {}

} // namespace tradegui
