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

#ifndef OPENSCOPE_PROBE_H
#define OPENSCOPE_PROBE_H

#include <array>
#include <string>
#include <utility>
#include <vector>
#include <thread>
#include <mutex>
#include <functional>

namespace OpenScope {

class Probe {
public:
    using MsgAvaliableCallback = std::function<void(std::string &&)>;

    struct DebugProbeInfo {
        DebugProbeInfo(std::string name, std::string serial) :
                name(std::move(name)), serial(std::move(serial)) {}

        std::string name;
        std::string serial;
    };

    Probe();
    ~Probe();

    static std::vector<std::string> listChips();
    static std::vector<DebugProbeInfo> listProbes();

    void open(std::size_t intf, const std::string &target);
    void wait();
    void close();
    void setRttMsgCallback(const MsgAvaliableCallback &cb);
    std::error_code startRtt(uint32_t start, uint32_t size);
    void stopRtt();
    void writeString(const std::string &str);
    void writeString(std::string &&str);

private:
    class impl;
    std::unique_ptr<impl> m_impl;
};

}

#endif //OPENSCOPE_PROBE_H
