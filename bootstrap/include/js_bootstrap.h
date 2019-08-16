#pragma once

#include <emscripten/val.h>
#include "js_types.h"
#include "js_callback.h"

namespace tc::js::globals {

namespace no_adl {
template<typename T>
struct Array : virtual tc::js::IJsBase {
    int length() { return m_emval["length"].template as<int>(); }

    void push(T item) { m_emval.call<void>("push", item); }

    auto operator[](int i) { return wrapper_detail::CPropertyProxy<T, int>(m_emval, i); }

    // Generator range. This adds operator() to array interface (which did not exist before), but it's ok.
    template<typename Fn>
    void operator()(Fn fn) noexcept {
        m_emval.call<void>("forEach", CScopedCallback([&](T value, emscripten::val, emscripten::val) noexcept {
            fn(tc_move(value));
        }));
    }
};

struct Console : virtual tc::js::IJsBase {
    // TODO: cannot return js_ref<js_function> because of overloads.
    // TODO: perfect forwarding?
    // TODO: allow passing options, ints, etc
    template<typename... Args>
    void log(js_ref<Args>... args) { m_emval.call<void>("log", args...); }
};
} // namespace no_adl

using no_adl::Array;
using no_adl::Console;

inline auto console() { return js_ref<Console>(emscripten::val::global("console")); }

} // namespace tc::js::globals
