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

#include "widget/filter_combo.h"

#include <utility>

#include <imgui.h>
#include <imgui_internal.h>
#include <imgui_stdlib.h>
#include <algorithm>

#include "lib/fts_fuzzy_match.h"
#include "font/IconsFontAwesome5.h"

namespace OpenScope {

FilterCombo::FilterCombo(std::string name) : Widget(std::move(name)) {}

void FilterCombo::drawContent() {
	ImGuiStyle& style = ImGui::GetStyle();
	ImGuiWindow* window = ImGui::GetCurrentWindow();
	std::string preview_value;
	bool is_filtering = !m_pattern.empty();

	static std::vector<std::pair<int, int>> item_score_vec;
	static bool is_already_open;
	static int focus_index;

	if (!m_items.empty()) {
		preview_value = m_items[m_cur_index];
	}

	if (ImGui::BeginCombo(
		getWidgetName().c_str(),
		preview_value.c_str(),
		ImGuiComboFlags_HeightLarge)) {

		ImGui::PushStyleColor(ImGuiCol_FrameBg, (ImVec4)ImColor(240, 240, 240, 255));
		ImGui::PushStyleColor(ImGuiCol_Text, (ImVec4)ImColor(0, 0, 0, 255));
		ImGui::PushItemWidth(-FLT_MIN);

		// Filter input
		if (!is_already_open) {
			m_pattern.clear();
			ImGui::SetKeyboardFocusHere();
			focus_index = m_cur_index;
		}
		is_already_open = true;

		const ImVec2 label_size = ImGui::CalcTextSize(ICON_FA_SEARCH, nullptr);
		ImGui::SetNextItemWidth(ImGui::CalcItemWidth() - label_size.x - style.FramePadding.x);
		if (ImGui::InputText("##FilterCombo_inputText", &m_pattern, 0)) {
			item_score_vec.clear();
			for (int i = 0; i < m_items.size(); i++) {
				int score = 0;
				if (fts::fuzzy_match(m_pattern.c_str(), m_items[i].c_str(), score))
					item_score_vec.emplace_back(i, score);
			}

			std::sort(item_score_vec.begin(), item_score_vec.end(),
				[](const auto& a, const auto& b) {
					return a.second > b.second;
				});
			if (!item_score_vec.empty()) focus_index = item_score_vec[0].first;
		}

		const ImVec2 search_icon_pos(
			ImGui::GetItemRectMax().x + style.FramePadding.x,
			ImGui::GetItemRectMax().y - style.FramePadding.y - window->CalcFontSize());
		ImGui::PopStyleColor(2);
		ImGui::RenderText(search_icon_pos, ICON_FA_SEARCH);

		int move_delta = 0;
		if (ImGui::IsKeyPressed(ImGuiKey_UpArrow)) --move_delta;
		if (ImGui::IsKeyPressed(ImGuiKey_DownArrow)) ++move_delta;

		if (is_filtering) {
			auto cur_score_index = std::find_if(item_score_vec.begin(), item_score_vec.end(),
				[](auto& value) {
					return value.first == focus_index;
				}) - item_score_vec.begin();
				if (!item_score_vec.empty()) {
					cur_score_index = (cur_score_index + move_delta + static_cast<int>(item_score_vec.size())) %
						static_cast<int>(item_score_vec.size());
					focus_index = item_score_vec[cur_score_index].first;
				}
		}
		else {
			if (!m_items.empty())
				focus_index = (focus_index + move_delta + static_cast<int>(m_items.size())) %
				static_cast<int>(m_items.size());
			else focus_index = 0;
		}

		if (ImGui::BeginListBox("##FilterCombo_itemList")) {
			std::size_t show_count;

			if (is_filtering) show_count = item_score_vec.size();
			else show_count = m_items.size();

			for (int i = 0; i < show_count; i++) {
				int idx = is_filtering ? item_score_vec[i].first : i;
				const bool item_selected = (idx == focus_index);
				const char* item_text = m_items[idx].c_str();

				ImGui::PushID((void*)(intptr_t)idx);
				if (ImGui::Selectable(item_text, item_selected)) {
					m_cur_index = idx;
					ImGui::CloseCurrentPopup();
				}

				if (item_selected) {
					ImGui::SetItemDefaultFocus();
					if (move_delta || ImGui::IsWindowAppearing()) {
						ImGui::SetScrollHereY();
					}
				}
				ImGui::PopID();
			}
			ImGui::EndListBox();

			if (ImGui::IsKeyPressedMap(ImGuiKey_Enter) | ImGui::IsKeyPressedMap(ImGuiKey_KeypadEnter)) {
				m_cur_index = focus_index;
				ImGui::CloseCurrentPopup();
			}
		}
		ImGui::EndCombo();
	}
	else {
		is_already_open = false;
	}
}

int FilterCombo::addItem(std::string&& item) {
	m_items.push_back(item);
	return 0;
}

const std::string& FilterCombo::currentItem()
{
	return m_items[m_cur_index];
}

} // OpenScope