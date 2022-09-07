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

#ifndef OPENSCOPE_RTT_VIEWER_H
#define OPENSCOPE_RTT_VIEWER_H

#include "widget/widget.h"
#include "widget/console.h"
#include "widget/symbol_table.h"

#include <functional>

namespace OpenScope {

class RttViewer : public Widget {
public:
    using StartCallback = std::function<std::error_code(uint32_t begin, uint32_t length)>;
    using StopCallback = std::function<void()>;

    explicit RttViewer();
    ~RttViewer();

    void drawContent() override;
    void setStartCallback(const StartCallback &cb);
    void setStopCallback(const StopCallback &cb);
    void append(std::string &&msg);

    static constexpr char WINDOW_NAME[] = "RTT Viewer";

private:
    Console m_console;
    SymbolTable m_symbol_table;

    std::string m_start_address;
    std::string m_size;
    bool m_is_running = false;
    StartCallback m_start_cb;
    StopCallback m_stop_cb;
};

} // OpenScope

#endif //OPENSCOPE_RTT_VIEWER_H
