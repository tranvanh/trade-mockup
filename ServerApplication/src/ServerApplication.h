#pragma once
#include <TradeCore/Market.h>
#include <TradeGUI/UIState.h>
#include <TradeGUI/UIStream.h>
#include <Toybox/Application.h>
#include <Toybox/CallbackOwner.h>
#include <Toybox/Server.h>
#include <ftxui/component/screen_interactive.hpp>

constexpr int SERVER_THREAD_COUNT = 6;

class ServerApplication
    : public toybox::CallbackOwner
    , public toybox::Application {
    TradeCore::Market          mStockMarket;
    toybox::Server             mServer;
    tradegui::UIState          mUIState;
    tradegui::UIStream         mUIStream;
    ftxui::ScreenInteractive   mScreen;

public:
    ServerApplication();
    void run()  override;
    void stop() override;

    tradegui::UIState&        uiState() { return mUIState; }
    ftxui::ScreenInteractive& screen()  { return mScreen; }

private:
    void processServerMessage(const std::string& msg);
    void onTradeExecuted(const TradeCore::Trade& trade);
};
