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

#ifndef OPENSCOPE_WINDOW_H
#define OPENSCOPE_WINDOW_H

#include <string>
#include <memory>

#include <imgui.h>
#include <GLFW/glfw3.h>

#include "lib/probe.h"
#include "widget/sidebar.h"
#include "widget/popup.h"
#include "widget/console.h"
#include "widget/rtt_viewer.h"

namespace OpenScope {

class Window {
public:
    Window();
    ~Window();

    void loop();

private:
    void initGLFW();
    void initImGui();
    void exitImGui();
    void exitGLFW();
    void frameBegin();
    void frame();
    void frameEnd();
    void loadFont();
    void createFullScreenDockSpace();
    void createFullScreenDockSpaceEnd();
    void resetLayout();

    static constexpr const char *MAIN_DOCKSPACE_ID = "MainDockSpace";

    GLFWwindow *m_window = nullptr;
    std::string m_window_title;
    Sidebar m_sidebar;
    Popup m_popup;
    RttViewer m_rtt_viewer;

    Probe m_probe;
};

} // OpenScope

#endif //OPENSCOPE_WINDOW_H
