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

#include "lib/probe.h"

#include "lib/event.hpp"

#include "probe_rust/src/lib.rs.h"

namespace OpenScope {

class Probe::impl {
public:
    explicit impl() : m_probe(probe_rust::new_probe()) {}

    ~impl() = default;

private:
    friend Probe;

    rust::Box<probe_rust::Probe> m_probe;
    std::thread m_rtt_thread;
    MsgAvaliableCallback m_rtt_cb;
};

Probe::Probe() : m_impl(std::make_unique<class Probe::impl>()) {}

Probe::~Probe() = default;

std::vector<std::string> Probe::listChips() {
    std::vector<std::string> result;

    auto chips = probe_rust::get_chips();
    for (auto &&name: chips) {
        result.push_back(std::string(name));
    }
    return result;
}

std::vector<Probe::DebugProbeInfo> Probe::listProbes() {
    std::vector<Probe::DebugProbeInfo> result;

    auto probes = probe_rust::list_probes();
    for (auto &&probe: probes) {
        result.emplace_back(std::string(probe.name), std::string(probe.serial));
    }

    return result;
}

void Probe::setRttMsgCallback(const MsgAvaliableCallback &cb) {
    m_impl->m_rtt_cb = cb;
}

void Probe::open(std::size_t intf, const std::string &target) {
    m_impl->m_probe->open(intf, target);
    EventManager::post<ProbeOpen>();
}

void Probe::close() {
    m_impl->m_probe->close();
    EventManager::post<ProbeClose>();
}

std::error_code Probe::startRtt(uint32_t start, uint32_t size) {
    std::error_code ec;
    return ec;
}

void OpenScope::Probe::stopRtt() {

}

void Probe::writeString(const std::string &str) {

}

void Probe::writeString(std::string &&str) {

}

void Probe::wait() {

}

}
