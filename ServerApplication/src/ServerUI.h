#pragma once
#include "ServerApplication.h"

class ServerUI {
    ServerApplication&        mApp;
    tradegui::UIState&        mState;
    ftxui::ScreenInteractive& mScreen;

public:
    explicit ServerUI(ServerApplication& app);
    void run();
};
