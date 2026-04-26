#include "ClientUI.h"
#include <TradeGUI/PanelBuilder.h>
#include <TradeCore/Order.h>
#include <ftxui/component/component.hpp>
#include <ftxui/component/event.hpp>
#include <ftxui/dom/elements.hpp>
#include <sstream>

using namespace ftxui;

ClientUI::ClientUI(ClientApplication& app)
    : mApp(app)
    , mState(app.uiState())
    , mScreen(app.screen()) {}

void ClientUI::run() {
    auto onSubmit = [this] {
        if (mInput.empty()) {
            return;
        }
        processInput();
        mScreen.PostEvent(Event::Custom);
    };

    auto cmdPanel   = tradegui::makeCmdHistoryPanel([this] { return mState.snapshotCmds(); });
    auto tradePanel = tradegui::makeTradeFeedPanel([this] { return mState.snapshotTrades(); });
    auto inputBox   = Input(&mInput, "subscribe | buy <p> <v> | sell <p> <v> | simulate");
    auto sendBtn    = Button("Send", onSubmit, ButtonOption::Ascii());

    auto inputRow = Container::Horizontal({inputBox, sendBtn});
    auto root     = Container::Vertical({inputRow});

    root = CatchEvent(root, [this, onSubmit](Event event) {
        if (event == Event::Return) {
            onSubmit();
            return true;
        }
        if (event == Event::Escape) {
            mScreen.Exit();
            return true;
        }
        return false;
    });

    auto renderer = Renderer(root, [this, cmdPanel, tradePanel, inputBox, sendBtn] {
        auto titleLine = hbox({
            text("Trading Client") | bold,
            text("  ID: " + std::to_string(mApp.id())) | dim,
        }) | hcenter;

        auto panels = hbox({
            cmdPanel->Render()   | flex,
            separator(),
            tradePanel->Render() | flex,
        });

        std::string simStatus  = mApp.isSimulating() ? "[SIM ON]" : "[SIM OFF]";
        auto statusLine = hbox({
            text(" "),
            text(simStatus) | (mApp.isSimulating() ? color(Color::Green) : color(Color::GrayDark)),
            text("  Press Esc to quit") | dim,
        });

        auto inputLine = hbox({
            text("> "),
            inputBox->Render() | flex,
            text(" "),
            sendBtn->Render(),
        });

        return vbox({
            titleLine,
            separator(),
            panels | flex,
            separator(),
            inputLine,
            statusLine,
        });
    });
    mScreen.Loop(renderer);
}

void ClientUI::processInput() {
    std::string cmd = mInput;
    mInput.clear();

    std::istringstream iss(cmd);
    std::string token;
    iss >> token;

    if (token == "subscribe") {
        mApp.subscribe();
        mState.pushCmd("> subscribe");
    } else if (token == "buy" || token == "sell") {
        double price = 0;
        int    vol   = 0;
        if (iss >> price >> vol && price > 0 && vol > 0) {
            TradeCore::OrderType type =
                (token == "buy") ? TradeCore::OrderType::BUY : TradeCore::OrderType::SELL;
            TradeCore::Order order(static_cast<int>(mApp.id()), type, static_cast<int>(price), vol);
            mState.pushCmd("> " + cmd);
            mApp.registerOrder(std::move(order));
        } else {
            mState.pushLog("[ERROR] Usage: buy|sell <price> <volume>");
        }
    } else if (token == "simulate") {
        mApp.toggleSimulate();
        mState.pushCmd("> simulate [" + std::string(mApp.isSimulating() ? "ON" : "OFF") + "]");
    } else {
        mState.pushLog("[ERROR] Unknown command: " + cmd);
    }
}