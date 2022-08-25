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

#ifndef OPENSCOPE_SIDEBAR_H
#define OPENSCOPE_SIDEBAR_H

#include "widget/widget.h"
#include "widget/filter_combo.h"

#include <functional>
#include <system_error>

namespace OpenScope {

class Sidebar : public Widget {
public:
	using ConnectCallback = std::function<std::error_code(const std::string&, const std::string&)>;

	Sidebar();
	~Sidebar() = default;

	void drawContent() override;
	void setConnectCallback(ConnectCallback f);

	static constexpr char WINDOW_NAME[] = "Debug Configuration";

private:
	FilterCombo m_target_combo;

	int m_intf_index = 0;
	bool m_infoPopupOpen = true;
	ConnectCallback m_connect_cb;
};

} // OpenScope

#endif //OPENSCOPE_SIDEBAR_H
