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
#include <csignal>
#include <memory>

#include "lib/fs.h"

namespace OpenScope {

OpenOcd::OpenOcd() : m_process(nullptr) {

}

constexpr std::array<std::pair<const char *, const char *>, 3> OpenOcd::INTERFACE_LIST = {
        {{"CMSIS-DAP", "interface/cmsis-dap.cfg"},
         {"ST-Link", "interface/stlink.cfg"},
         {"J-Link", "interface/jlink.cfg"}}};

std::vector<std::string> OpenOcd::listTarget() {
    std::vector<std::string> result;
#ifdef _DEBUG
    std::string target_script_path(
            bp::search_path("openocd").remove_filename().generic_string() + "/../scripts/target");
#else
    std::string target_script_path("./openocd/script/target");
#endif
    for (auto &&f: fs::listDirectory(target_script_path)) {
        auto filename = f.stem().generic_string();
        result.push_back(filename);
    }
    return result;
}

void OpenOcd::startProcess(const std::string &intf, const std::string &target) {
    bp::ipstream is;
    std::error_code ec;
    m_process = std::make_unique<bp::child>("E:/C/OpenScope/cmake-build-debug/bin/openocd/bin/openocd.exe",
                                            "-f", intf, "-f", target,
                                            bp::std_err > is, ec);
}


}
