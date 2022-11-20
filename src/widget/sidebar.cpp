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

#include "font/IconsFontAwesome5.h"
#include "lib/event.hpp"
#include "lib/probe.h"
#include "widget/popup.h"

namespace OpenScope {

Sidebar::Sidebar() :
        Widget(WINDOW_NAME, true),
        m_target_combo("Target") {
    m_probes = Probe::listProbes();
    for (auto &name: Probe::listChips()) {
        m_target_combo.addItem(std::move(name));
    }

    EventManager::subscribe<ProbeOpen>(this, [&] {
      m_is_running = true;
    });
    EventManager::subscribe<ProbeClose>(this, [&] {
      m_is_running = false;
    });
}

Sidebar::~Sidebar() {
    EventManager::unsubscribe<ProbeOpen>(this);
    EventManager::unsubscribe<ProbeClose>(this);
}

void Sidebar::drawContent() {

    ImGui::Combo("Interface", &m_intf_index, [](void *data, int index, const char **text) {
      auto &probes = reinterpret_cast<Sidebar *>(data)->m_probes;
      *text = probes[index].name.c_str();
      return true;
    }, this, static_cast<int>(m_probes.size()));
    ImGui::SameLine();
    if (ImGui::Button(ICON_FA_SYNC_ALT)) {
        m_probes = Probe::listProbes();
    }

    m_target_combo.drawContent();

    ImGui::Spacing();
    if (!m_is_running) {
        if (ImGui::Button("Connect")) {
            assert(m_connect_cb);
            try {
                m_connect_cb(m_intf_index, m_target_combo.currentItem());
            } catch (const std::exception &e) {
                Popup::showInfo(e.what());
            }
        }
    } else {
        if (ImGui::Button("Close")) {
            assert(m_close_cb);
            try {
                m_close_cb();
            } catch (const std::exception &e) {
                Popup::showInfo(e.what());
            }
        }
    }
}

void Sidebar::setConnectCallback(const ConnectCallback &f) {
    m_connect_cb = f;
}

void Sidebar::setCloseCallback(const TerminateCallback &f) {
    m_close_cb = f;
}

} // OpenScope