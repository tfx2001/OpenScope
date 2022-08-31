#include "widget/console.h"

#include <utility>

#include <imgui.h>
#include <imgui_stdlib.h>


namespace OpenScope {

Console::Console(std::string name, bool is_window) :
        Widget(std::move(name), is_window),
        m_scroll_down(false) {}

void Console::drawContent() {
    std::lock_guard guard(m_lock);

    ImGui::PushStyleColor(ImGuiCol_FrameBg, ImVec4(ImColor(0, 0, 0, 0)));
    ImGui::PushStyleVar(ImGuiStyleVar_FrameBorderSize, 1);
    ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(0, 0));

    ImGuiListClipper clipper(static_cast<int>(m_message.size()));
    while (clipper.Step())
        for (int i = clipper.DisplayStart; i < clipper.DisplayEnd; i++)
            ImGui::TextUnformatted(m_message[i].c_str());

    if (m_scroll_down)
        ImGui::SetScrollHereY();
    m_scroll_down = false;

    ImGui::PopStyleVar(2);
    ImGui::PopStyleColor();
}

void Console::appendLine(std::string &&output) {
    std::lock_guard guard(m_lock);

    m_message.push_back(output);
    m_scroll_down = true;
}

}