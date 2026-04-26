#include "TradeGUI/PanelBuilder.h"
#include <ftxui/dom/elements.hpp>
#include <ftxui/component/component.hpp>

using namespace ftxui;

constexpr size_t MAX_LINES = 10;
namespace tradegui {
    static Component makeScrollableList(
        std::function<std::deque<std::string>()> snapshot,
        const std::string &title) {
        return Renderer([snapshot, title] {
            auto lines = snapshot();
            Elements elems;
            elems.reserve(MAX_LINES);

            for (size_t i = lines.size() - MAX_LINES; i < lines.size(); ++i) {
                elems.push_back(text(lines[i]));
            }

            if (elems.empty()) {
                elems.push_back(text("") | dim);
            }

            return window(
                text(title),
                vbox(std::move(elems)) | yframe | vscroll_indicator | focusPositionRelative(0, 1)
            );
        });
    }

    Component makeTradeFeedPanel(std::function<std::deque<std::string>()> snapshot) {
        return makeScrollableList(std::move(snapshot), "Trade Feed");
    }

    Component makeLogPanel(std::function<std::deque<std::string>()> snapshot) {
        return makeScrollableList(std::move(snapshot), "Log");
    }

    Component makeCmdHistoryPanel(std::function<std::deque<std::string>()> snapshot) {
        return makeScrollableList(std::move(snapshot), "Command History");
    }

    Component makeConnectionsPanel(std::function<std::vector<unsigned short>()> snapshot) {
        return Renderer([snapshot] {
            auto conns = snapshot();
            Elements elems;
            elems.reserve(conns.size());
            for (auto port: conns)
                elems.push_back(text("  port " + std::to_string(port)));
            if (elems.empty())
                elems.push_back(text("  (none)") | dim);

            return window(
                text("Connections"),
                vbox(std::move(elems)) | yframe | vscroll_indicator
            );
        });
    }
} // namespace tradegui
