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
#include <regex>

#include <fmt/format.h>

#include "lib/fs.h"
#include "lib/event.hpp"

#include <boost/process/io.hpp>
#include <boost/process/search_path.hpp>
#include <boost/asio/ip/tcp.hpp>
#include <boost/asio/read_until.hpp>
#include <boost/asio/write.hpp>
#include <boost/algorithm/string.hpp>

#ifdef _WIN32
#include <boost/process/windows.hpp>
#endif

using boost::asio::ip::tcp;

namespace OpenScope {

constexpr std::array<std::pair<const char *, const char *>, 3> OpenOcd::INTERFACE_LIST = {
        {{"CMSIS-DAP", "interface/cmsis-dap.cfg"},
         {"ST-Link", "interface/stlink.cfg"},
         {"J-Link", "interface/jlink.cfg"}}};

OpenOcd::~OpenOcd() {
    terminate();
    wait();
}

std::vector<std::string> OpenOcd::listTarget() {
    std::vector<std::string> result;
#ifdef OS_LINUX
    std::string target_script_path("/usr/share/openocd/scripts/target");
#elif defined(OS_WINDOWS) && !defined(NDEBUG)
    std::string target_script_path(
            bp::search_path("openocd").remove_filename().generic_string() + "/../scripts/target");
#else
    std::string target_script_path("openocd/scripts/target");
#endif

    for (auto &&f: fs::listDirectory(target_script_path)) {
        auto filename = f.stem().generic_string();
        result.push_back(std::move(filename));
    }
    return result;
}

void OpenOcd::setOpenOcdMsgCallback(const MsgAvaliableCallback &cb) {
    m_opencod_cb = cb;
}

void OpenOcd::setRttMsgCallback(const MsgAvaliableCallback &cb) {
    m_rtt_cb = cb;
}

std::error_code OpenOcd::startProcess(const std::string &intf, const std::string &target) {
    std::error_code ec;
#if !defined(NDEBUG) || defined(unix)
    std::string exec_path(bp::search_path("openocd").generic_string());
#else
    std::string exec_path("openocd/bin/openocd.exe");
#endif

    wait();

    m_read_stream = std::make_unique<bp::ipstream>();
    m_process = bp::child(exec_path,
                          "-f", intf, "-f", target, "-c", "adapter speed 10000;", bp::std_err > *m_read_stream,
#ifdef _WIN32
            bp::windows::hide,
#endif
                          ec);
    if (!ec) {
        m_openocd_thread = std::thread(&OpenOcd::openocdThread, this);
    }
    return ec;
}

std::error_code OpenOcd::startRtt(uint32_t start, uint32_t size) {
    tcp::socket s(m_io_context);
    tcp::endpoint ep(asio::ip::address::from_string("127.0.0.1"), 4444);

    try {
        s.connect(ep);

        std::string buf;
        std::size_t n;
        n = asio::read_until(s, asio::dynamic_buffer(buf), '\n');
        buf.erase(0, n);
        s.write_some(asio::buffer(fmt::format("rtt setup {} {} SEGGER\\ RTT; rtt stop; rtt start;\r", start, size)));
        n = asio::read_until(s, asio::dynamic_buffer(buf), '\n');
        buf.erase(0, n);
        n = asio::read_until(s, asio::dynamic_buffer(buf), '\n');
        buf.erase(0, n);
        // Check if control block found
        n = asio::read_until(s, asio::dynamic_buffer(buf), '\n');
        if (buf.find("at") != std::string::npos) {
            buf.erase(0, n);
            s.write_some(asio::buffer("rtt server stop 0; rtt server start 0 0; exit;\r"));
            n = asio::read_until(s, asio::dynamic_buffer(buf), '\n');
            buf.erase(0, n);
            n = asio::read_until(s, asio::dynamic_buffer(buf), '\n');
            // Get port number
            std::regex port_regex("\\d+");
            std::smatch m;
            if (std::regex_search(buf, m, port_regex)) {
                // Wait last thread exit
                if (m_rtt_thread.joinable()) m_rtt_thread.join();
                // Start new thread for rtt
                m_rtt_thread = std::thread(&OpenOcd::rttThread, this, std::stoi(m[0]));
            }
        } else {
            buf.erase(0, n);
            s.write_some(asio::buffer("exit;\r"));
            n = asio::read_until(s, asio::dynamic_buffer(buf), '\n');
        }
        s.close();
        return {};
    }
    catch (const boost::system::system_error &error) {
        s.close();
        return std::error_code(error.code());
    }
}

void OpenScope::OpenOcd::stopRtt() {
    boost::system::error_code ec;
    tcp::socket s(m_io_context);

    tcp::endpoint ep(asio::ip::address::from_string("127.0.0.1"), 4444);
    s.connect(ep, ec);

    std::string buf;
    std::size_t n;
    // Read OpenOCD banner
    n = asio::read_until(s, asio::dynamic_buffer(buf), '\n');
    buf.erase(0, n);
    asio::write(s, asio::buffer("rtt server stop 0;\r"));
    n = asio::read_until(s, asio::dynamic_buffer(buf), '\n');
    buf.erase(0, n);
    s.close();
}

void OpenOcd::wait() {
    if (m_openocd_thread.joinable())
        m_openocd_thread.join();
    if (m_rtt_thread.joinable())
        m_rtt_thread.join();
}

void OpenOcd::terminate() {
    if (m_process.running())
        m_process.terminate();
}

void OpenOcd::openocdThread() {
    std::string line;

    EventManager::post<OpenOcdStart>();
    while (std::getline(*m_read_stream, line)) {
        m_opencod_cb(std::move(line));
    }
    EventManager::post<OpenOcdExit>();
}

void OpenOcd::rttThread(int port) {
    tcp::socket s(m_io_context);
    tcp::endpoint ep(asio::ip::address::from_string("127.0.0.1"), port);
    std::string buf;
    size_t n;

    EventManager::post<RttStart>();
    try {
        s.connect(ep);
        asio::write(s, asio::buffer("start"));
        while ((n = asio::read_until(s, asio::dynamic_buffer(buf), '\n'))) {
            m_rtt_cb(boost::algorithm::trim_copy(buf.substr(0, n)));
            buf.erase(0, n);
        }
    }
    catch (const boost::system::system_error &) {
        s.close();
    }
    EventManager::post<RttExit>();
}

}