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

#ifndef OPENSCOPE_SYMBOL_TABLE_H
#define OPENSCOPE_SYMBOL_TABLE_H

#include "widget/widget.h"

namespace OpenScope {

class SymbolTable : public Widget {
public:
    explicit SymbolTable(std::string name);
    ~SymbolTable() = default;

    void drawContent() override;
};

} // OpenScope

#endif //OPENSCOPE_SYMBOL_TABLE_H
