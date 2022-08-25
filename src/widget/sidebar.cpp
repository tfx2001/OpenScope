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

#include "lib/openocd.h"
#include "widget/popup.h"

namespace OpenScope {

Sidebar::Sidebar() :
	Widget(WINDOW_NAME, true),
	m_target_combo("Target") {

	for (auto& filename : OpenOcd::listTarget()) {
		m_target_combo.addItem(std::move(filename));
	}
}

void Sidebar::drawContent() {
	ImGui::BeginChild("config combo", ImVec2(0, -ImGui::GetFrameHeightWithSpacing()));
	ImGui::Combo("Interface", &m_intf_index, [](void* data, int index, const char** text) {
		*text = OpenOcd::INTERFACE_LIST[index].first;
		return true;
		}, nullptr, static_cast<int>(OpenOcd::INTERFACE_LIST.size()));
	m_target_combo.drawContent();

	ImGui::EndChild();

	if (ImGui::Button("Connect"))
		if (m_connect_cb) {
			auto ec = m_connect_cb(
				OpenOcd::INTERFACE_LIST[m_intf_index].second,
				"target/" + m_target_combo.currentItem() + ".cfg");
			if (ec) {
				Popup::showInfoPopup(fmt::format("OpenOCD start failed: \n{}", ec.message()));
			}
		}
}

void Sidebar::setConnectCallback(ConnectCallback f)
{
	m_connect_cb = f;
}

} // OpenScope