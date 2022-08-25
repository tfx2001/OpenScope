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

#include <boost/process/windows.hpp>

#include "lib/fs.h"

namespace OpenScope {

constexpr std::array<std::pair<const char*, const char*>, 3> OpenOcd::INTERFACE_LIST = {
		{{"CMSIS-DAP", "interface/cmsis-dap.cfg"},
		 {"ST-Link",   "interface/stlink.cfg"},
		 {"J-Link",    "interface/jlink.cfg"}} };

OpenOcd::~OpenOcd() {
	if (m_process.running())
		m_process.terminate();
	wait();
}

std::vector<std::string> OpenOcd::listTarget() {
	std::vector<std::string> result;
#ifdef _DEBUG
	std::string target_script_path(
		bp::search_path("openocd").remove_filename().generic_string() + "/../scripts/target");
#else
	std::string target_script_path("./openocd/script/target");
#endif
	for (auto&& f : fs::listDirectory(target_script_path)) {
		auto filename = f.stem().generic_string();
		result.push_back(filename);
	}
	return result;
}

std::error_code OpenOcd::startProcess(const std::string& intf,
	const std::string& target,
	MsgAvaliableCallback&& cb) {
	std::error_code ec;

#ifdef _DEBUG
	std::string exec_path(bp::search_path("openocd").generic_string());
#else
	std::string exec_path("./openocd/bin/openocd");
#endif

	m_cb = cb;

	m_read_stream = std::make_unique<bp::ipstream>();
	m_process = bp::child(exec_path,
		"-f", intf, "-f", target,
		bp::std_out > *m_read_stream, bp::std_err > *m_read_stream, bp::std_in < bp::null, ec);

	if (!ec) {
		m_thread = std::thread([&] { this->threadEntry(); });
	}

	return ec;
}

void OpenOcd::wait() {
	m_thread.join();
}

void OpenOcd::threadEntry()
{
	std::string line;

	m_is_running = true;

	while (std::getline(*m_read_stream.get(), line) && !line.empty()) {
		m_cb(std::move(line));
	}

	m_is_running = false;
}

}
