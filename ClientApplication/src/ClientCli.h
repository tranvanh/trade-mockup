#pragma once
#include <TradeGUI/CliBase.h>

class ClientApplication;

class ClientCli : public TradeGUI::CliBase {
    ClientApplication& mApp;

public:
    explicit ClientCli(ClientApplication& app);
    void run();

private:
    enum class Mode { ORDER_ENTRY, SIMULATE };

    Mode selectMode();
    void runOrderRepl();
};
