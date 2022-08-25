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

#include "window/window.h"

#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>
#include <imgui_internal.h>
#include <fmt/format.h>

#include "font/OpenSans-Regular.hpp"

namespace OpenScope {

Window::Window() : m_console(Console::OPENOCD_WINDOW_NAME) {
    this->initGLFW();
    this->initImGui();

    EventManager::subscribe<WindowCreate>(this, [this](const std::string &) { this->resetLayout(); });

    m_sidebar.setConnectCallback([&](auto &intf, auto &target) {
      return m_openocd.startProcess(intf, target);
    });
    m_sidebar.setTerminateCallback([&] { m_openocd.terminate(); });
    m_openocd.setNewMsgCallback([&](auto &&msg) { m_console.appendLine(std::forward<std::string>(msg)); });
}

Window::~Window() {
    EventManager::unsubscribe<WindowCreate>(this);

    this->exitImGui();
    this->exitGLFW();
}

void Window::loop() {
    while (!glfwWindowShouldClose(this->m_window)) {
        this->frameBegin();
        this->frame();
        this->frameEnd();
    }
}

void Window::frameBegin() {
    glfwPollEvents();

    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();
    ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4(ImColor(21, 22, 23)));

    createFullScreenDockSpace();
}

void Window::frame() {
    EventManager::post<DrawEvent>();
}

void Window::frameEnd() {
    createFullScreenDockSpaceEnd();
    ImGui::PopStyleColor();
    ImGui::Render();

    int display_w, display_h;
    glfwGetFramebufferSize(this->m_window, &display_w, &display_h);
    glViewport(0, 0, display_w, display_h);
    glClear(GL_COLOR_BUFFER_BIT);
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

    glfwSwapBuffers(this->m_window);
}

void Window::initGLFW() {
    glfwSetErrorCallback([](int error, const char *desc) {
      fmt::print(stderr, "GLFW Error [{}] : {}", error, desc);
    });

    if (!glfwInit()) {
        fmt::print("Failed to initialize GLFW!");
        std::abort();
    }

#if defined(OS_MACOS)
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
#else
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
#endif
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

    this->m_window_title = "ImHex";
    this->m_window = glfwCreateWindow(1240, 700, this->m_window_title.c_str(), nullptr, nullptr);
    glfwSetWindowUserPointer(this->m_window, this);

    if (this->m_window == nullptr) {
        fmt::print("Failed to create window!");
        std::abort();
    }

    glfwMakeContextCurrent(this->m_window);
    glfwSwapInterval(1);

    glfwSetWindowSizeCallback(this->m_window, [](GLFWwindow *window, int height, int width) {
      auto win = static_cast<Window *>(glfwGetWindowUserPointer(window));
      win->frameBegin();
      win->frame();
      win->frameEnd();
    });

    glfwSetWindowTitle(this->m_window, "OpenScope");
}

void Window::initImGui() {
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGui::StyleColorsDark();

    ImGuiIO &io = ImGui::GetIO();
    ImGuiStyle &style = ImGui::GetStyle();

    io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;

    style.WindowMenuButtonPosition = ImGuiDir_None;
    style.FrameRounding = 3.0f;
    style.PopupRounding = 3.0f;

    ImGui_ImplGlfw_InitForOpenGL(this->m_window, true);
#if defined(OS_MACOS)
    ImGui_ImplOpenGL3_Init("#version 150");
#else
    ImGui_ImplOpenGL3_Init("#version 130");
#endif

    loadFont();
}

void Window::exitGLFW() {
    glfwDestroyWindow(this->m_window);
    glfwTerminate();
}

void Window::exitImGui() {
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
}

void Window::createFullScreenDockSpace() {
    ImGuiViewport *viewport = ImGui::GetMainViewport();
    ImGui::SetNextWindowPos(viewport->Pos);

    ImVec2 viewportSize = viewport->Size;
    ImGui::SetNextWindowSize(viewportSize);
    ImGui::SetNextWindowViewport(viewport->ID);
    ImGui::SetNextWindowBgAlpha(0.0f);

    ImGuiWindowFlags windowFlags = ImGuiWindowFlags_NoDocking;
    windowFlags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize |
                   ImGuiWindowFlags_NoMove;
    windowFlags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;
    windowFlags |= ImGuiWindowFlags_MenuBar;

    ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
    ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
    static bool p_open = true;
    std::string windowsTitle = "Main window (title bar invisible)";
    ImGui::Begin(windowsTitle.c_str(), &p_open, windowFlags);
    ImGui::PopStyleVar(3);
    ImGuiID dockSpaceId = ImGui::GetID(MAIN_DOCKSPACE_ID);
    ImGui::DockSpace(dockSpaceId, ImVec2(0.0f, 0.0f), 0);
}

void Window::createFullScreenDockSpaceEnd() {
    ImGui::End();
}

void Window::loadFont() {
    ImGuiIO &io = ImGui::GetIO();

    ImFontConfig config;
    config.FontDataOwnedByAtlas = false;

    io.Fonts->Clear();
    // OpenSans
    io.Fonts->AddFontFromMemoryTTF((void *) OpenScope::OpenSans_Regular_data,
                                   OpenScope::OpenSans_Regular_size, 18, &config);
    Widget::loadIconFont(16);

    ImGui_ImplOpenGL3_DestroyFontsTexture();
    ImGui_ImplOpenGL3_CreateFontsTexture();
}

void Window::resetLayout() {
    ImGuiID dockSpaceId = ImGui::GetID(MAIN_DOCKSPACE_ID);

    ImGui::DockBuilderRemoveNode(dockSpaceId);
    ImGui::DockBuilderAddNode(dockSpaceId, ImGuiDockNodeFlags_DockSpace);
    ImGui::DockBuilderSetNodeSize(dockSpaceId, ImGui::GetMainViewport()->Size);

    ImGuiID sidebarId;
    ImGuiID mainWindowId;
    ImGuiID openocdOutputId;

    sidebarId = ImGui::DockBuilderSplitNode(
            dockSpaceId, ImGuiDir_Left, 0.25f, nullptr, &mainWindowId);
    openocdOutputId = ImGui::DockBuilderSplitNode(
            mainWindowId, ImGuiDir_Down, 0.25f, nullptr, &mainWindowId);

    ImGui::DockBuilderDockWindow(Sidebar::WINDOW_NAME, sidebarId);
    ImGui::DockBuilderDockWindow(Console::OPENOCD_WINDOW_NAME, openocdOutputId);
    ImGui::DockBuilderFinish(dockSpaceId);
}

} // OpenScope
