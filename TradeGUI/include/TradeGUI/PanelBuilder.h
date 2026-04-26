#pragma once
#include <deque>
#include <functional>
#include <string>
#include <vector>
#include <ftxui/component/component.hpp>

namespace tradegui {

ftxui::Component makeTradeFeedPanel(
    std::function<std::deque<std::string>()> snapshot);

ftxui::Component makeLogPanel(
    std::function<std::deque<std::string>()> snapshot);

ftxui::Component makeConnectionsPanel(
    std::function<std::vector<unsigned short>()> snapshot);

ftxui::Component makeCmdHistoryPanel(
    std::function<std::deque<std::string>()> snapshot);

} // namespace tradegui
