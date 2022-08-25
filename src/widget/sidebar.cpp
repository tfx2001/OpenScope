/*
 * Copyright 2022 tfx2001 <tfx2001@outlook.com>
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include "widget/sidebar.h"

#include <imgui.h>

#include "lib/openocd.h"
#include "lib/fs.h"

#include "widget/popup.h"

namespace OpenScope {

Sidebar::Sidebar() :
        Widget(WINDOW_NAME, true),
        m_target_combo("Target") {

    for (auto &filename: OpenOcd::listTarget()) {
        m_target_combo.addItem(std::move(filename));
    }
}

void Sidebar::drawContent() {
    ImGui::BeginChild("config combo", ImVec2(0, -ImGui::GetFrameHeightWithSpacing()));
    ImGui::Combo("Interface", &m_intf_index, [](void *data, int index, const char **text) {
      *text = OpenOcd::INTERFACE_LIST[index].first;
      return true;
    }, nullptr, static_cast<int>(OpenOcd::INTERFACE_LIST.size()));
    m_target_combo.drawContent();

    ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(0, 0));
    ImGuiListClipper clipper(10000000);
    while (clipper.Step())
        for (int i = clipper.DisplayStart; i < clipper.DisplayEnd; i++)
            ImGui::Text("%i The quick brown fox jumps over the lazy dog", i);
    ImGui::PopStyleVar();

    ImGui::EndChild();

    if (ImGui::Button("Connect"))
        Popup::showInfoPopup("Connecting...");
}

} // OpenScope