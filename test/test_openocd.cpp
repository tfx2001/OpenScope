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
#ifndef OPENSCOPE_TEST_OPENOCD_H

#include <iostream>
#include <lib/openocd.h>

using namespace OpenScope;

int main(int, char**) {
	OpenOcd openocd;

	openocd.startProcess(OpenOcd::INTERFACE_LIST[1].second, "target/stm32h7x.cfg");
	return 0;
}

#define OPENSCOPE_TEST_OPENOCD_H

#endif //OPENSCOPE_TEST_OPENOCD_H
