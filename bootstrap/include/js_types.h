#pragma once
#include <type_traits>
#include <emscripten/val.h>
#include "noncopyable.h"
#include "type_traits.h"
#include "tc_move.h"
#include "js_ref.h"

#include <stdio.h>
namespace tc::js {
namespace wrapper_detail {
namespace no_adl {
template<typename T, typename Name>
struct CPropertyProxy : tc::nonmovable {
    CPropertyProxy(emscripten::val& m_emval, Name m_name)
        : m_emval(m_emval)
        , m_name(m_name) {
    }

    // TODO: it's still possible to use incorrectly:
    // auto x = foo["bar"]; // Presumably copy.
    // foo["bar"] = undefined;
    // baz(tc_move(x)); // 'Moving' value to baz.
    operator T() && { return m_emval[m_name].template as<T>(); }
    T const& operator=(T const& value) && {
        m_emval.set(m_name, emscripten::val(value));
        return value;
    }

private:
    emscripten::val& m_emval;
    Name m_name;
};
} // namespace no_adl
using no_adl::CPropertyProxy;
} // namespace wrapper_detail

namespace no_adl {
struct IAny : virtual IJsBase {
};

template<typename, typename = void, typename = void>
struct IJsFunction {};

template<typename R, typename... Args, typename TThis, typename TArgs>
struct IJsFunction<R(Args...), TThis, TArgs> : virtual IJsBase {
    static_assert(tc::is_decayed<R>::value);
    static_assert(std::conjunction<tc::is_decayed<Args>...>::value);
    static_assert(tc::is_decayed<TThis>::value);
    static_assert(tc::is_decayed<TArgs>::value);
};

template<typename R, typename... Args>
struct IJsFunction<R(Args...), void, void> : virtual IJsBase {
    static_assert(tc::is_decayed<R>::value);
    static_assert(std::conjunction<tc::is_decayed<Args>...>::value);

    R operator()(Args... args) noexcept {
        return m_emval(tc_move(args)...).template as<R>();
    }
};
} // namespace no_adl
using no_adl::IAny;
using no_adl::IJsFunction;
} // namespace tc::js
