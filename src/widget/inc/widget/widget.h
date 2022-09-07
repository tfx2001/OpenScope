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

#ifndef OPENSCOPE_WIDGET_H
#define OPENSCOPE_WIDGET_H

#include <string>

namespace OpenScope {

class Widget {
public:
    Widget(std::string name, bool is_window = false, bool closeable = false);
    ~Widget();

    void drawWindow();
    virtual void drawContent() = 0;
    bool &getWindowOpenState();
    bool getWindowOpenState() const;

    static void loadIconFont(float pixel_size);

protected:
    const std::string &getWidgetName() const { return m_name; }

private:
    std::string m_name;
    bool m_is_window;
    bool m_closable;
    bool m_window_open = false;
    bool m_first_frame = true;
};

} // OpenScope

#endif //OPENSCOPE_WIDGET_H
