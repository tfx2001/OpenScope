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

#ifndef OPENSCOPE_POPUP_H
#define OPENSCOPE_POPUP_H

#include "widget/widget.h"

#include <string>
#include <system_error>

namespace OpenScope {

class Popup : public Widget {
public:
    explicit Popup();
    ~Popup();

    void drawContent() override;

    static void showInfo(const std::string &message);
    static void showError(const std::string &message);
    static void showError(const std::error_code &ec);

private:
    static std::string s_info_message;
    static std::string s_error_message;
    static bool s_open_info;
    static bool s_open_error;
};

}

#endif //OPENSCOPE_POPUP_H
