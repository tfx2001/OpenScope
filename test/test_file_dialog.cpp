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

#include <iostream>

#include "lib/platform.h"

int main(int, char **) {
    std::filesystem::path path;
    if (OpenScope::platform::openFile(path)) {
        std::cout << path << std::endl;

        if (std::filesystem::exists(path)) {
            std::cout << "File exists." << std::endl;
        } else {
            std::cout << "File not exists." << std::endl;
        }
    } else {
        std::cout << "User cancelled." << std::endl;
    }

    return 0;
}