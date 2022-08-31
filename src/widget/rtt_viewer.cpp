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

#include "widget/rtt_viewer.h"

#include <imgui.h>
#include <imgui_stdlib.h>
#include <fmt/format.h>

#include "widget/popup.h"
#include "lib/event.hpp"

namespace OpenScope {


RttViewer::RttViewer() :
        Widget(WINDOW_NAME, true),
        m_console("RTT Output"),
        m_start_address("0x20000000"),
        m_size("128") {
    EventManager::subscribe<RttStart>(this, [&] { m_is_running = true; });
    EventManager::subscribe<RttExit>(this, [&] { m_is_running = false; });
}

RttViewer::~RttViewer() = default;

void RttViewer::drawContent() {
    ImVec2 size = ImGui::GetContentRegionAvail();
    size.y -= ImGui::GetFrameHeightWithSpacing() + ImGui::GetStyle().ItemSpacing.y * 2;
    ImGui::BeginChild("RTT Output", size);
    m_console.drawContent();
    ImGui::EndChild();

    ImGui::Separator();
    ImGui::Spacing();
    ImGui::PushItemWidth(86);
    ImGui::InputText("Begin", &m_start_address);
    ImGui::SameLine();
    ImGui::InputText("Size (in KB)", &m_size, ImGuiInputTextFlags_CharsDecimal);
    ImGui::PopItemWidth();
    ImGui::SameLine();

    if (!m_is_running) {
        if (ImGui::Button("Start")) {
            if (m_start_cb) {
                if (m_start_address.size() && m_size.size()) {
                    auto ec = m_start_cb(std::stoi(m_start_address, nullptr, 16), std::stoi(m_size) * 1024);
                    if (ec) {
                        Popup::showError(ec);
                    }
                } else {
                    Popup::showInfo(
                            "Please set the RAM begin address and size where the RTT control block is located.");
                }
            }
        }
    } else {
        if (ImGui::Button("Stop")) {
            if (m_stop_cb) m_stop_cb();
        }
    }
}

void RttViewer::setStartCallback(const StartCallback &cb) {
    m_start_cb = cb;
}

void RttViewer::setStopCallback(const StopCallback &cb) {
    m_stop_cb = cb;
}

void RttViewer::append(std::string &&msg) {
    m_console.appendLine(std::forward<std::string>(msg));
}


} // OpenScope