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

#include "widget/widget.h"

#include <utility>

#include <imgui.h>

#include "lib/event.hpp"
#include "font/fa5-solid-900.h"
#include "font/IconsFontAwesome5.h"

namespace OpenScope {

Widget::Widget(std::string name, bool is_window) :
        m_name(std::move(name)),
        m_is_window(is_window) {
    if (m_is_window) {
        EventManager::subscribe<DrawEvent>(this, [this] { this->drawWindow(); });
    }
}

Widget::~Widget() {
    EventManager::unsubscribe<DrawEvent>(this);
}

void Widget::drawWindow() {
    if (ImGui::Begin(m_name.c_str())) {
        drawContent();
    }
    ImGui::End();

    if (m_first_frame) {
        EventManager::post<WindowCreate>(m_name);
        m_first_frame = false;
    }
}

void Widget::loadIconFont(float pixel_size) {
    ImFontConfig config;
    ImGuiIO &io = ImGui::GetIO();

    static const ImWchar icons_ranges[] = {ICON_MIN_FA, ICON_MAX_FA, 0};
    config.FontDataOwnedByAtlas = false;
    config.MergeMode = true;
    config.PixelSnapH = true;
    io.Fonts->AddFontFromMemoryTTF((void *) OpenScope::fa5_solid_900_data,
                                   OpenScope::fa5_solid_900_size, pixel_size,
                                   &config, icons_ranges);
}

} // OpenScope