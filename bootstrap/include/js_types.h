#pragma once
#include <type_traits>
#include "type_traits.h"
#include "tc_move.h"
#include "js_ref.h"

namespace tc::js {
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
