#pragma once

#include <emscripten/val.h>
#include "js_types.h"

namespace tc::js::globals {

namespace no_adl {
template<typename T>
struct Array : virtual tc::js::IJsBase {
    int length() { return m_emval["length"].template as<int>(); }

    void push(T item) { m_emval.call<void>("push", item); }

    auto operator[](int i) { return wrapper_detail::CPropertyProxy<T, int>(m_emval, i); }
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
