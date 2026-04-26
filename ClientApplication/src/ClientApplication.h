#pragma once
#include "StockMarketGenerator.h"
#include <TradeCore/Order.h>
#include <TradeGUI/UIState.h>
#include <TradeGUI/UIStream.h>
#include <Toybox/Application.h>
#include <Toybox/Client.h>
#include <ftxui/component/screen_interactive.hpp>

constexpr int CLIENT_THREAD_COUNT = 4;

class ClientApplication : public toybox::Application {
    uint                         mId;
    tradegui::UIState            mUIState;
    tradegui::UIStream           mUIStream;
    toybox::Client               mClient;
    StockMarketGenerator         mGenerator;
    ftxui::ScreenInteractive     mScreen;

public:
    ClientApplication();
    void run()  override;
    void stop() override;

    void registerOrder(TradeCore::Order order) const;
    void subscribe();
    void toggleSimulate();
    bool isSimulating() const;

    uint                      id()      const { return mId; }
    tradegui::UIState&        uiState() { return mUIState; }
    ftxui::ScreenInteractive& screen()  { return mScreen; }

private:
    static uint generateId();
};
