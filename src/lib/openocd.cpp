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

#include "lib/openocd.h"

#include <iostream>
#include <memory>

#include "lib/fs.h"
#include "lib/event.hpp"

#ifdef _WIN32
#include <boost/process/windows.hpp>
#endif

namespace OpenScope {

constexpr std::array<std::pair<const char *, const char *>, 3> OpenOcd::INTERFACE_LIST = {
        {{"CMSIS-DAP", "interface/cmsis-dap.cfg"},
         {"ST-Link", "interface/stlink.cfg"},
         {"J-Link", "interface/jlink.cfg"}}};

OpenOcd::~OpenOcd() {
    if (m_process.running())
        m_process.terminate();
    wait();
}

std::vector<std::string> OpenOcd::listTarget() {
    std::vector<std::string> result;
#if !defined(NDEBUG) && defined(_WIN32)
    std::string target_script_path(
            bp::search_path("openocd").remove_filename().generic_string() + "/../scripts/target");
#else
    std::string target_script_path("/usr/share/openocd/scripts/target");
#endif

    for (auto &&f: fs::listDirectory(target_script_path)) {
        auto filename = f.stem().generic_string();
        result.push_back(filename);
    }
    return result;
}

void OpenOcd::setNewMsgCallback(const MsgAvaliableCallback &cb) {
    m_cb = cb;
}

std::error_code OpenOcd::startProcess(const std::string &intf, const std::string &target) {
    std::error_code ec;
#if !defined(NDEBUG) || defined(unix)
    std::string exec_path(bp::search_path("openocd").generic_string());
#else
    std::string exec_path("./openocd/bin/openocd");
#endif

    wait();

    m_read_stream = std::make_unique<bp::ipstream>();
    m_process = bp::child(exec_path,
                          "-f", intf, "-f", target, bp::std_err > *m_read_stream,
#ifdef _WIN32
                          bp::windows::hide,
#endif
                          ec);

    if (!ec) {
        m_thread = std::thread([&] { this->threadEntry(); });
    }
    return ec;
}

void OpenOcd::wait() {
    if (m_thread.joinable())
        m_thread.join();
}

void OpenOcd::terminate() {
    if (m_process.running()) m_process.terminate();
}

void OpenOcd::threadEntry() {
    std::string line;

    EventManager::post<OpenOcdStart>();


    while (std::getline(*m_read_stream, line) && !line.empty()) {
        m_cb(std::move(line));
    }

    EventManager::post<OpenOcdExit>();
}

}
