#pragma once

#include <emscripten/val.h>
#include <utility>
#include <type_traits>
#include "explicit_cast.h"
#include "type_traits.h"
#include "range.h"
#include "js_types.h"
#include "js_callback.h"
#include "tc_move.h"

namespace tc::js::globals {

namespace no_adl {
template<typename> struct _js_Array;
template<typename> struct _js_ReadonlyArray;
struct _js_Console;

template<typename T> using Array = js_ref<_js_Array<T>>;
template<typename T> using ReadonlyArray = js_ref<_js_ReadonlyArray<T>>;
using Console = js_ref<_js_Console>;

template<typename T>
struct _js_Array : virtual IObject {
    static_assert(IsJsInteropable<T>::value);

    auto length() { return tc::explicit_cast<int>(_getProperty<double>("length")); }

    auto push(T const& item) { return _call<void>("push", item); }

    auto operator[](int i) && { return _getProperty<T>(i); }

    void _setIndex(int i, T value) { _setProperty(i, tc_move(value)); }

    // Generator range. This adds operator() to array interface (which did not exist before), but it's ok.
    template<typename Fn>
    auto operator()(Fn fn) noexcept {
        return _call<void>("forEach", js_lambda_wrap([&](T value, js_unknown, js_unknown) noexcept {
            fn(tc_move(value));
        }));
    }

    template<typename Rng, typename = std::enable_if_t<tc::is_explicit_castable<T, tc::range_value_t<Rng>&&>::value>>
    static Array<T> _construct(Rng&& rng) noexcept {
        Array<T> result(emscripten::val::array());
        tc::for_each(rng, [&](auto&& value) {
            result->push(tc::explicit_cast<T>(std::forward<decltype(value)>(value)));
        });
        return result;
    }
};

template<typename T>
struct _js_ReadonlyArray : virtual IObject {
    static_assert(IsJsInteropable<T>::value);

    auto length() { return tc::explicit_cast<int>(_getProperty<double>("length")); }

    auto operator[](int i) { return _getProperty<T>(i); }

    // Generator range. This adds operator() to array interface (which did not exist before), but it's ok.
    template<typename Fn>
    auto operator()(Fn fn) noexcept {
        return _call<void>("forEach", js_lambda_wrap([&](T value, js_unknown, js_unknown) noexcept {
            fn(tc_move(value));
        }));
    }

    template<typename Rng, typename = std::enable_if_t<tc::is_explicit_castable<T, tc::range_value_t<Rng>&&>::value>>
    static ReadonlyArray<T> _construct(Rng&& rng) noexcept {
        return ReadonlyArray<T>(
            tc::explicit_cast<Array<T>>(std::forward<Rng>(rng)).getEmval()
        );
    }
};

struct _js_Console : virtual IObject {
    template<typename... Args>
    auto log(Args&&... args) {
        static_assert((IsJsInteropable<tc::remove_cvref_t<Args>>::value && ...));
        return _call<void>("log", std::forward<Args>(args)...);
    }
};
} // namespace no_adl

using no_adl::Array;
using no_adl::ReadonlyArray;
using no_adl::Console;

inline auto console() { return Console(emscripten::val::global("console")); }

} // namespace tc::js::globals
