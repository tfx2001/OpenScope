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
#include <fmt/format.h>

#include "lib/event.hpp"
#include "lib/openocd.h"
#include "widget/popup.h"

namespace OpenScope {

Sidebar::Sidebar() :
        Widget(WINDOW_NAME, true),
        m_target_combo("Target") {

    for (auto &filename: OpenOcd::listTarget()) {
        m_target_combo.addItem(std::move(filename));
    }

    EventManager::subscribe<OpenOcdStart>(this, [&] {
      std::lock_guard lock(m_lock);
      m_is_running = true;
    });
    EventManager::subscribe<OpenOcdExit>(this, [&] {
      std::lock_guard lock(m_lock);
      m_is_running = false;
    });
}

Sidebar::~Sidebar() {
    EventManager::unsubscribe<OpenOcdStart>(this);
    EventManager::unsubscribe<OpenOcdExit>(this);
}

void Sidebar::drawContent() {
    std::lock_guard lock(m_lock);

    ImGui::Combo("Interface", &m_intf_index, [](void *data, int index, const char **text) {
      *text = OpenOcd::INTERFACE_LIST[index].first;
      return true;
    }, nullptr, static_cast<int>(OpenOcd::INTERFACE_LIST.size()));
    m_target_combo.drawContent();

    ImGui::Spacing();
    if (!m_is_running) {
        if (ImGui::Button("Connect")) {
            if (m_connect_cb) {
                auto ec = m_connect_cb(
                        OpenOcd::INTERFACE_LIST[m_intf_index].second,
                        "target/" + m_target_combo.currentItem() + ".cfg");
                if (ec) {
                    Popup::showError(ec);
                }
            }
        }
    } else {
        if (ImGui::Button("Terminate")) {
            if (m_terminate_cb) {
                m_terminate_cb();
            }
        }
    }
}

void Sidebar::setConnectCallback(const ConnectCallback &f) {
    m_connect_cb = f;
}

void Sidebar::setTerminateCallback(const TerminateCallback &f) {
    m_terminate_cb = f;
}

} // OpenScope