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

#include "widget/popup.h"

#include <utility>

#include <imgui.h>

#include "lib/event.hpp"

namespace OpenScope {

std::string Popup::s_popup_message;
bool Popup::s_open_info;

Popup::Popup() : Widget("Popup") {
    EventManager::subscribe<DrawEvent>(this, [this] { this->drawContent(); });
}

Popup::~Popup() {
    EventManager::unsubscribe<DrawEvent>(this);
}

void Popup::drawContent() {
    ImVec2 center = ImGui::GetMainViewport()->GetCenter();

    if (s_open_info) {
        ImGui::OpenPopup("Info");
        s_open_info = false;
    }

    // Info popup
    ImGui::SetNextWindowSizeConstraints(ImVec2(300, 100), ImVec2(600, 300));
    ImGui::SetNextWindowPos(center, ImGuiCond_Appearing, ImVec2(0.5f, 0.5f));
    if (ImGui::BeginPopupModal("Info", nullptr, ImGuiWindowFlags_AlwaysAutoResize)) {
        ImGui::TextWrapped("%s", s_popup_message.c_str());
        ImGui::NewLine();
        ImGui::Separator();
        if (ImGui::Button("Ok") || ImGui::IsKeyDown(ImGuiKey_Escape))
            ImGui::CloseCurrentPopup();
        ImGui::EndPopup();
    }
}

void Popup::showInfo(const std::string &message) {
    s_popup_message = message;
    s_open_info = true;
}

}