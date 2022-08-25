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

#ifndef OPENSCOPE_EVENT_HPP
#define OPENSCOPE_EVENT_HPP

#include <iostream>
#include <functional>
#include <unordered_map>
#include <utility>

#define EVENT_DEF(EVENT_NAME, ...)                                              \
    struct EVENT_NAME : public Event<__VA_ARGS__>                               \
    {                                                                           \
        constexpr static auto id = EventId();                                   \
        explicit EVENT_NAME(Callback func) noexcept : Event(std::move(func)) {} \
    }

namespace OpenScope {

class EventId {
public:
    explicit constexpr EventId(const char *func = __builtin_FUNCTION(), uint32_t line = __builtin_LINE()) : m_hash() {
        this->m_hash = line ^ 987654321;
        for (const char *c = func; *c != '\0'; c++) {
            this->m_hash = (this->m_hash >> 5) | (this->m_hash << 27);
            this->m_hash ^= *c;
        }
    }

    bool operator==(const EventId &rhs) const {
        return this->m_hash == rhs.m_hash;
    };

    std::size_t operator()(const EventId &e) {
        return e.m_hash;
    }

private:
    friend struct std::hash<EventId>;

    uint32_t m_hash;
};

}

template<>
struct std::hash<OpenScope::EventId> {
    size_t operator()(const OpenScope::EventId &id) const {
        return id.m_hash;
    }
};

namespace OpenScope {

struct EventBase {
public:
    EventBase() = default;
};

template<typename... Params>
struct Event : public EventBase {
    using Callback = std::function<void(Params...)>;

    explicit Event(Callback func) noexcept: m_func(std::move(func)) {}

    void operator()(Params... params) const noexcept {
        this->m_func(params...);
    }

private:
    Callback m_func;
};


class EventManager {
public:
    using SubscriptionPool = std::unordered_multimap<EventId, EventBase *, std::hash<EventId>>;
    using SubsToken = std::unordered_multimap<void *, SubscriptionPool::iterator>;

    template<typename E>
    static SubscriptionPool::iterator subscribe(typename E::Callback cb) {
        return m_subs.insert(std::make_pair(E::id, new E(cb)));
    }

    template<typename E>
    static void subscribe(void *token, typename E::Callback cb) {
        m_tokens.insert(std::make_pair(token, subscribe<E>(cb)));
    }

    template<typename E, typename ...Params>
    static void post(Params &&...args) noexcept {
        auto range = m_subs.equal_range(E::id);
        for (auto i = range.first; i != range.second; ++i) {
            (*static_cast<E *>(i->second))(args...);
        }
    }

    static void unsubscribe(SubscriptionPool::iterator iter) noexcept {
        m_subs.erase(std::move(iter));
    }

    template<typename E>
    static void unsubscribe(void *token) noexcept {
        auto iter = std::find_if(m_tokens.begin(), m_tokens.end(), [&](auto &item) {
          return item.first == token && item.second->first == E::id;
        });

        if (iter != m_tokens.end()) {
            m_subs.erase(iter->second);
            m_tokens.erase(iter);
        }
    }

    static void clean() {
        m_subs.clear();
        m_tokens.clear();
    }

private:
    static SubscriptionPool m_subs;
    static SubsToken m_tokens;
};

// Draw event
EVENT_DEF(DrawEvent);
// Widget event
EVENT_DEF(WindowCreate, const std::string &);
// Event for test
EVENT_DEF(TestEvent1, int);
EVENT_DEF(TestEvent2, int);

}

#endif //OPENSCOPE_EVENT_HPP
