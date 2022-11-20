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

#include "lib/event.hpp"

#include <cassert>
#include <fmt/format.h>

using namespace OpenScope;

void postEvent(int value) {
    EventManager::post<TestEvent1>(value);
    EventManager::post<TestEvent2>(value);
}

int main(int, char **) {
    void *TOKEN = reinterpret_cast<void *>(1);
    int value1 = 0;
    int value2 = 6;

    EventManager::subscribe<TestEvent1>(TOKEN, [&](int i) {
      value1 += i;
    });
    EventManager::subscribe<TestEvent2>(TOKEN, [&](int i) {
      value2 += i;
    });
    postEvent(5);

    // Unsubscribe TestEvent for value2
    EventManager::unsubscribe<TestEvent2>(TOKEN);
    postEvent(7);

    float value = 1.23789287384f;
    fmt::print("Hello {:f}", value);

    assert(value1 == 12);   // 0 + 5 + 7
    assert(value2 == 11);   // 6 + 5
    return 0;
}
