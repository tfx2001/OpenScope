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

#ifndef OPENSCOPE_CONSOLE_H
#define OPENSCOPE_CONSOLE_H

#include "widget/widget.h"

#include <string>
#include <vector>
#include <mutex>

namespace OpenScope {

class Console : public Widget {
public:
    explicit Console(std::string name, bool is_window = false);
    ~Console() = default;

    void drawContent() override;
    void append(std::string &&output);

    static constexpr char OPENOCD_WINDOW_NAME[] = "OpenOCD Output";

private:
    std::string m_message;
    bool m_scroll_down;
    std::mutex m_lock;
};

} // OpenScope

#endif //OPENSCOPE_CONSOLE_H