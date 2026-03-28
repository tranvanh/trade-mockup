#pragma once
#include <TradeGUI/CliBase.h>
#include <Toybox/ThreadSafeQueue.h>
#include <TradeCore/Trade.h>
#include <string>
#include <vector>

class ServerApplication;

class ServerCli : public TradeGUI::CliBase {
    ServerApplication& mApp;

    enum class View { EVENTS, TRADES };
    View mCurrentView{View::EVENTS};

    toybox::ThreadSafeQueue<std::string> mEventQueue;
    toybox::ThreadSafeQueue<std::string> mTradeQueue;
    std::vector<std::string>             mEventHistory;
    std::vector<std::string>             mTradeHistory;
    static constexpr int                 MAX_LOG = 200;

public:
    explicit ServerCli(ServerApplication& app);
    void run();

    // Called from background threads (ServerApplication callbacks)
    void appendEvent(std::string msg);
    void appendTrade(const TradeCore::Trade& trade);

private:
    // Setup — port uses "fallback on bad input" semantics, not inputInt retry loop
    void promptConfig(int& port, bool& filled);

    // View management
    void switchView(View view);
    void printViewHeader(View view);

    // Drain pending queue entries into history (CLI thread only)
    void drainQueues();

    // Command loop
    std::string currentPrompt() const;
    void        handleCommand(const std::string& cmd);
};
