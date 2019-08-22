#pragma once

#include <emscripten/val.h>
#include <string>
#include "explicit_cast.h"
#include "range.h"
#include "js_types.h"
#include "js_callback.h"
#include "tc_move.h"

namespace tc::js::globals {

namespace no_adl {
template<typename T>
struct Array : virtual IUnknown {
    static_assert(IsJsInteropable<T>::value);

    auto length() { return _getProperty<int>("length"); }

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

    template<typename Rng>
    static js_ref<Array> _construct(Rng&& rng) noexcept {
        js_ref<Array> result(emscripten::val::array());
        tc::for_each(rng, [&](auto&& value) {
            result->push(tc::explicit_cast<T>(std::forward<decltype(value)>(value)));
        });
        return result;
    }
};

template<typename T>
struct ReadonlyArray : virtual IUnknown {
    static_assert(IsJsInteropable<T>::value);

    auto length() { return _getProperty<int>("length"); }

    auto operator[](int i) { return _getProperty<T>(i); }

    // Generator range. This adds operator() to array interface (which did not exist before), but it's ok.
    template<typename Fn>
    auto operator()(Fn fn) noexcept {
        return _call<void>("forEach", js_lambda_wrap([&](T value, js_unknown, js_unknown) noexcept {
            fn(tc_move(value));
        }));
    }

    template<typename Rng>
    static js_ref<ReadonlyArray> _construct(Rng&& rng) noexcept {
        return js_ref<ReadonlyArray>(
            tc::explicit_cast<js_ref<Array<T>>>(std::forward<Rng>(rng)).getEmval()
        );
    }
};

struct String : virtual IUnknown {
    auto length() { return _getProperty<int>("length"); }

    // TODO: clang does not see String::operator std::string() when return type is deduced, report bug?
    // See https://godbolt.org/z/y8_jBG
    explicit operator std::string() { return _getEmval().template as<std::string>(); }

    template<typename Rng>
    static auto _construct(Rng&& rng) noexcept {
        // TODO: avoid allocating std::string (we may have to duplicate parts of embind)
        return emscripten::val(tc::explicit_cast<std::string>(std::forward<Rng>(rng)));
    }
};

struct Console : virtual IUnknown {
    // TODO: cannot return js_ref<js_function> because of overloads.
    // TODO: perfect forwarding?
    // TODO: allow passing options, ints, etc
    template<typename... Args>
    auto log(js_ref<Args>... args) { return _call<void>("log", args...); }
};
} // namespace no_adl

using no_adl::Array;
using no_adl::ReadonlyArray;
using no_adl::String;
using no_adl::Console;

inline auto console() { return js_ref<Console>(emscripten::val::global("console")); }

} // namespace tc::js::globals
