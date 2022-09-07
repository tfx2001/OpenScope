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

#include "widget/symbol_table.h"

#include <imgui.h>
#include <imgui_stdlib.h>
#include <fmt/format.h>
#include <libdwarf.h>

#include "widget/popup.h"
#include "lib/platform.h"

namespace OpenScope {


SymbolTable::SymbolTable(std::string name) : Widget(std::move(name), true, true) {

}

void SymbolTable::drawContent() {
    auto path = m_path.generic_string();
    ImGui::InputText("File", &path, ImGuiInputTextFlags_ReadOnly);
    ImGui::SameLine();
    if (ImGui::Button("Open")) {
        if (platform::openFile(m_path)) {
            loadElf();
        }
    }
}

void SymbolTable::loadElf() {

}

} // OpenScope