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

#ifndef OPENSCOPE_FILTER_COMBO_H
#define OPENSCOPE_FILTER_COMBO_H

#include "widget/widget.h"

#include <vector>

namespace OpenScope {

class FilterCombo : public Widget {
public:
    explicit FilterCombo(std::string name);
    ~FilterCombo() = default;

    void drawContent() override;
    int addItem(std::string &&item);
    const std::string &currentItem();

private:
    std::vector<std::string> m_items;
    std::string m_pattern;
    int m_cur_index = 0;
};

} // OpenScope

#endif //OPENSCOPE_FILTER_COMBO_H
