#pragma once
#include "ClientApplication.h"

class ClientUI {
    ClientApplication&        mApp;
    tradegui::UIState&        mState;
    ftxui::ScreenInteractive& mScreen;
    std::string               mInput;

public:
    explicit ClientUI(ClientApplication& app);
    void run();
private:
    void processInput();
};
