#pragma once

#include <emscripten/val.h>
#include <string>
#include "range.h"
#include "js_types.h"
#include "js_callback.h"

namespace tc::js::globals {

namespace no_adl {
template<typename T>
struct Array : virtual IJsBase {
    static_assert(IsJsInteropable<T>::value);

    auto length() { return _getProperty<int>("length"); }

    auto push(T const& item) { return _call<void>("push", item); }

    auto operator[](int i) { return _propertyProxy<T>(i); }

    // Generator range. This adds operator() to array interface (which did not exist before), but it's ok.
    template<typename Fn>
    auto operator()(Fn fn) noexcept {
        return _call<void>("forEach", js_lambda_wrap([&](T value, js_ref<IAny>, js_ref<IAny>) noexcept {
            fn(tc_move(value));
        }));
    }
};

template<typename T>
struct ReadonlyArray : virtual IJsBase {
    static_assert(IsJsInteropable<T>::value);

    auto length() { return _getProperty<int>("length"); }

    auto operator[](int i) { return _getProperty<T>(i); }

    // Generator range. This adds operator() to array interface (which did not exist before), but it's ok.
    template<typename Fn>
    auto operator()(Fn fn) noexcept {
        return _call<void>("forEach", js_lambda_wrap([&](T value, js_ref<IAny>, js_ref<IAny>) noexcept {
            fn(tc_move(value));
        }));
    }
};

struct String : virtual IJsBase {
    auto length() { return _getProperty<int>("length"); }

    explicit operator auto() { return _getEmval().template as<std::string>(); }
};

struct Console : virtual IJsBase {
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

namespace tc::no_adl {
template<typename T>
struct SConversions<tc::js::js_ref<tc::js::globals::Array<T>>> {
    template<typename Rng>
    static auto fn(Rng&& rng) { // TODO: MAYTHROW?
        tc::js::js_ref<tc::js::globals::Array<T>> result(emscripten::val::array());
        tc::for_each(rng, [&](auto&& value) {
            result->push(tc::explicit_cast<T>(std::forward<decltype(value)>(value)));
        });
        return result;
    }
};

template<typename T>
struct SConversions<tc::js::js_ref<tc::js::globals::ReadonlyArray<T>>> {
    template<typename Rng>
    static auto fn(Rng&& rng) { // TODO: MAYTHROW?
        return tc::js::js_ref<tc::js::globals::ReadonlyArray<T>>(
            tc::explicit_cast<tc::js::js_ref<tc::js::globals::Array<T>>>(std::forward<Rng>(rng)).get()
        );
    }
};

template<>
struct SConversions<tc::js::js_ref<tc::js::globals::String>> {
    template<typename Rng>
    static auto fn(Rng&& rng) { // TODO: MAYTHROW?
        // TODO: avoid allocating std::string (we may have to duplicate parts of embind)
        return tc::js::js_ref<tc::js::globals::String>(
            emscripten::val(
                tc::explicit_cast<std::string>(std::forward<Rng>(rng))
            )
        );
    }
};
} // namespace tc::no_adl
