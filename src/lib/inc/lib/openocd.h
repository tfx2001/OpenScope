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

#ifndef OPENSCOPE_OPENOCD_H
#define OPENSCOPE_OPENOCD_H

#include <array>
#include <string>
#include <vector>
#include <thread>

#include <SDKDDKVer.h>
#include <boost/process.hpp>

namespace OpenScope {

namespace bp = boost::process;

class OpenOcd {
public:
	using MsgAvaliableCallback = std::function<void(std::string&&)>;

	OpenOcd() = default;
	~OpenOcd();

	static const std::array<std::pair<const char*, const char*>, 3> INTERFACE_LIST;
	static std::vector<std::string> listTarget();

	std::error_code startProcess(const std::string& intf, const std::string& target, MsgAvaliableCallback&& cb);
	void wait();

private:
	bp::child m_process;
	std::unique_ptr<bp::ipstream> m_read_stream;
	std::thread m_thread;
	MsgAvaliableCallback m_cb;
	bool m_is_running;

	void threadEntry();
};

}


#endif //OPENSCOPE_OPENOCD_H
