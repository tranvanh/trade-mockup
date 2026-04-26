#include "ServerUI.h"
#include <TradeGUI/PanelBuilder.h>
#include <ftxui/component/component.hpp>
#include <ftxui/component/event.hpp>
#include <ftxui/dom/elements.hpp>

using namespace ftxui;

ServerUI::ServerUI(ServerApplication& app)
    : mApp(app)
    , mState(app.uiState())
    , mScreen(app.screen()) {}

void ServerUI::run() {
    auto connPanel  = tradegui::makeConnectionsPanel([this] { return mState.snapshotConnections(); });
    auto tradePanel = tradegui::makeTradeFeedPanel([this] { return mState.snapshotTrades(); });
    auto logPanel   = tradegui::makeLogPanel([this] { return mState.snapshotLogs(); });

    auto topArea = Container::Horizontal({
        connPanel,
        tradePanel,
    });

    auto root = Container::Vertical({
        topArea,
        logPanel,
    });

    root = CatchEvent(root, [&](Event event) {
        if (event == Event::Escape) {
            mScreen.Exit();
            return true;
        }
        return false;
    });

    auto renderer = Renderer(root, [&] {
        auto titleLine = text("Trade Server") | bold | hcenter;

        auto topRow = hbox({
            connPanel->Render()  | size(WIDTH, EQUAL, 32),
            separator(),
            tradePanel->Render() | flex,
        });

        auto connCount  = mState.snapshotConnections().size();
        auto tradeCount = mState.snapshotTotalTrades();
        auto statusLine = hbox({
            text(" Connections: " + std::to_string(connCount)),
            text("  |  Trades: " + std::to_string(tradeCount)) | dim,
            text("  |  Press Esc to quit") | dim,
        });

        return vbox({
            titleLine,
            separator(),
            topRow | flex,
            separator(),
            logPanel->Render() | size(HEIGHT, EQUAL, 12),
            separator(),
            statusLine,
        });
    });

    mScreen.Loop(renderer);
}
