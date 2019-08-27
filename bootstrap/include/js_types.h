#pragma once
#include <type_traits>
#include <emscripten/val.h>
#include <emscripten/wire.h>
#include "explicit_cast.h"
#include "noncopyable.h"
#include "type_list.h"
#include "type_traits.h"
#include "tc_move.h"

namespace tc::js {
namespace no_adl {
template<typename, typename = void>
struct IsJsInteropable : std::false_type {};

template<typename T>
struct IsJsInteropable<
    T,
    std::enable_if_t<tc::type::find_unique<
        tc::type::list<
            void,
            char, signed char, unsigned char,
            signed short, unsigned short,
            signed int, unsigned int,
            signed long, unsigned long,
            float, double,
            bool
        >,
        T
    >::found>
> : std::true_type {};

template<typename T>
struct IsJsIntegralEnum : std::false_type {};

struct js_unknown {
    js_unknown(emscripten::val const& _emval) : m_emval(_emval) {}
    js_unknown(emscripten::val&& _emval) : m_emval(tc_move(_emval)) {}

    emscripten::val getEmval() const& { return m_emval; }
    emscripten::val&& getEmval() && { return tc_move(m_emval); }

    template<typename T, typename = std::enable_if_t<IsJsInteropable<tc::remove_cvref_t<T>>::value>>
    js_unknown(T&& value) : m_emval(std::forward<T>(value)) {
    }

    template<typename U, typename = std::enable_if_t<IsJsInteropable<tc::remove_cvref_t<U>>::value>>
    explicit operator U() const& { return m_emval.template as<U>(); }

    template<typename U, typename = std::enable_if_t<IsJsInteropable<tc::remove_cvref_t<U>>::value>>
    explicit operator U&&() && { return tc_move(m_emval).template as<U>(); }

    explicit operator bool() const {
        return !!m_emval;
    }

private:
	emscripten::val m_emval;
};

struct js_undefined {
    js_undefined() {}
    js_undefined(emscripten::val const& emval) {
        _ASSERT(emval.isUndefined());
    }
    emscripten::val getEmval() const& { return emscripten::val::undefined(); }
};

struct js_null {
    js_null() {}
    js_null(emscripten::val const& emval) {
        _ASSERT(emval.isNull());
    }
    emscripten::val getEmval() const& { return emscripten::val::null(); }
};

struct js_string;
} // namespace no_adl
using no_adl::IsJsInteropable;
using no_adl::IsJsIntegralEnum;
using no_adl::js_unknown;
using no_adl::js_undefined;
using no_adl::js_null;
using no_adl::js_string;

namespace js_union_detail {
namespace no_adl {
template<typename... Args>
struct CFindValueType {
    static inline constexpr auto has_value_type = false;
};

template<typename T>
struct CFindValueType<js_undefined, T> {
    static inline constexpr auto has_value_type = true;
    using value_type = T;
};

template<typename T>
struct CFindValueType<js_null, T> {
    static inline constexpr auto has_value_type = true;
    using value_type = T;
};
} // namespace no_adl
using no_adl::CFindValueType;
} // namespace js_union_detail

namespace no_adl {
// TODO: optimize by providing JS-side toWireType/fromWireType for integrals/bools and getting rid of emscripten::val
template<typename... Args>
struct js_union : js_union_detail::CFindValueType<Args...> {
    static inline constexpr auto instantiated = true;

    static_assert(1 < sizeof...(Args));
    static_assert((IsJsInteropable<Args>::value && ...));
    static_assert((!std::is_same<Args, js_unknown>::value && ...));

    using ListArgs = tc::type::list<Args...>;

    static inline constexpr auto has_undefined = tc::type::find_unique<ListArgs, js_undefined>::found;
    static inline constexpr auto has_null = tc::type::find_unique<ListArgs, js_null>::found;
    static inline constexpr auto has_string = tc::type::find_unique<ListArgs, js_string>::found;
    static inline constexpr auto has_bool = tc::type::find_unique<ListArgs, bool>::found;
    static inline constexpr auto has_number = tc::type::find_unique_if<ListArgs, std::is_arithmetic>::found;

    using js_union_detail::CFindValueType<Args...>::has_value_type;

    js_union(emscripten::val const& _emval) : m_emval(_emval) { validateAny(); }
    js_union(emscripten::val&& _emval) : m_emval(tc_move(_emval)) { validateAny(); }

    emscripten::val getEmval() const& { return m_emval; }
    emscripten::val&& getEmval() && { return tc_move(m_emval); }

    template<typename T, typename = std::enable_if_t<!has_value_type && IsJsInteropable<tc::remove_cvref_t<T>>::value && (std::is_convertible<T, Args>::value || ...)>>
    explicit js_union(T&& value) : m_emval(value) {}

    template<typename T = js_union, std::enable_if_t<T::has_undefined && !T::has_null && T::has_value_type>* = nullptr>
    js_union() : js_union(emscripten::val::undefined()) {}

    template<typename T = js_union, std::enable_if_t<!T::has_undefined && T::has_null && T::has_value_type>* = nullptr>
    js_union() : js_union(emscripten::val::null()) {}

    template<typename T, typename U = js_union, typename = std::enable_if_t<IsJsInteropable<tc::remove_cvref_t<T>>::value && std::is_convertible<T, typename U::value_type>::value>>
    js_union(T&& value) : m_emval(value) {}

    template<typename T, typename = std::enable_if_t<IsJsInteropable<tc::remove_cvref_t<T>>::value && (std::is_convertible<Args, T>::value || ...)>>
    explicit operator T() const& { validate<tc::remove_cvref_t<T>>(); return m_emval.template as<T>(); }

private:
    template<typename T>
    struct CArrowProxy {
        T m_value;
        CArrowProxy(T&& value) : m_value(tc_move(value)) {}
        T* operator->() { return &m_value; }
        T const* operator->() const { return &m_value; }
    };

public:
    std::enable_if_t<has_value_type, typename js_union::value_type> operator*() const {
        static_assert(IsJsInteropable<tc::remove_cvref_t<typename js_union::value_type>>::value && (std::is_convertible<Args, typename js_union::value_type>::value || ...));
        return operator typename js_union::value_type();
    }

    std::enable_if_t<has_value_type, CArrowProxy<typename js_union::value_type>> operator->() const {
        static_assert(IsJsInteropable<tc::remove_cvref_t<typename js_union::value_type>>::value && (std::is_convertible<Args, typename js_union::value_type>::value || ...));
        return operator typename js_union::value_type();
    }

    explicit operator bool() const {
        return !!m_emval;
    }

private:
    emscripten::val m_emval;

    template<typename T>
    void validate() const {
        if constexpr (std::is_same<T, js_undefined>::value) {
            _ASSERT(m_emval.isUndefined());
        }
        if constexpr (std::is_same<T, js_null>::value) {
            _ASSERT(m_emval.isNull());
        }
        if constexpr (std::is_same<T, js_string>::value) {
            _ASSERT(m_emval.isString());
        }
        if constexpr (std::is_same<T, bool>::value) {
            _ASSERT(m_emval.isTrue() || m_emval.isFalse());
        }
        if constexpr (std::is_arithmetic<T>::value) {
            _ASSERT(m_emval.isNumber());
        }
    }

    void validateAny() const {
        if constexpr (!has_undefined) {
            _ASSERT(!m_emval.isUndefined());
        }
        if constexpr (!has_null) {
            _ASSERT(!m_emval.isNull());
        }
        if constexpr (!has_string) {
            _ASSERT(!m_emval.isString());
        }
        if constexpr (!has_bool) {
            _ASSERT(!m_emval.isTrue());
            _ASSERT(!m_emval.isFalse());
        }
        if constexpr (!has_number) {
            _ASSERT(!m_emval.isNumber());
        }
    }
};

struct js_string final {
    explicit js_string(emscripten::val const& _emval) noexcept : m_emval(_emval) {
        _ASSERT(m_emval.isString());
    }
    explicit js_string(emscripten::val&& _emval) noexcept : m_emval(tc_move(_emval)) {
        _ASSERT(m_emval.isString());
    }

    emscripten::val getEmval() const& noexcept { return m_emval; }
    emscripten::val getEmval() && noexcept { return tc_move(m_emval); }

    template<typename Rng, typename = std::enable_if_t<
        !std::is_same<emscripten::val, tc::remove_cvref_t<Rng>>::value &&
        !std::is_same<js_unknown, tc::remove_cvref_t<Rng>>::value &&
        !tc::is_instance_or_derived<js_union, Rng>::value
    >>
    explicit js_string(Rng&& rng) noexcept : m_emval(
        // TODO: avoid allocating std::string (we may have to duplicate parts of embind)
        tc::explicit_cast<std::string>(std::forward<Rng>(rng))
    ) {
    }

    int length() { return m_emval["length"].as<int>(); }

    explicit operator bool() const {
        return !!m_emval;
    }

    explicit operator std::string() { return m_emval.template as<std::string>(); }

private:
    emscripten::val m_emval;
};
} // namespace no_adl
using no_adl::js_union;
template<typename T>
using js_optional = js_union<js_undefined, T>;

template<>
struct IsJsInteropable<js_unknown> : std::true_type {};

template<>
struct IsJsInteropable<js_undefined> : std::true_type {};

template<>
struct IsJsInteropable<js_null> : std::true_type {};

template<typename... Args>
struct IsJsInteropable<js_union<Args...>> : std::true_type {
    static_assert(js_union<Args...>::instantiated);
};

template<>
struct IsJsInteropable<js_string> : std::true_type {};

template<typename T>
struct IsJsInteropable<T, std::enable_if_t<IsJsIntegralEnum<T>::value>> : std::true_type {};

namespace emscripten_interop_detail {
namespace no_adl {
template<typename T, typename = void>
struct IsEmvalWrapper : std::false_type {};

template<typename T>
struct IsEmvalWrapper<T, std::enable_if_t<tc::type::find_unique<tc::type::list<js_unknown, js_undefined, js_null, js_string>, T>::found>> : std::true_type {};

template<typename... Args>
struct IsEmvalWrapper<js_union<Args...>, std::enable_if_t<IsJsInteropable<js_union<Args...>>::value>> : std::true_type {};
}
using no_adl::IsEmvalWrapper;
} // namespace emscripten_interop_detail
} // namespace tc::js

// Custom marshalling
namespace emscripten::internal {
    template<typename T>
    struct TypeID<T, std::enable_if_t<tc::js::emscripten_interop_detail::IsEmvalWrapper<tc::remove_cvref_t<T>>::value>> {
        static constexpr TYPEID get() {
            return TypeID<val>::get();
        }
    };

    template<typename T>
    struct BindingType<T, std::enable_if_t<tc::js::emscripten_interop_detail::IsEmvalWrapper<T>::value>> {
        typedef typename BindingType<emscripten::val>::WireType WireType;

        static WireType toWireType(T const& js) {
            return BindingType<emscripten::val>::toWireType(js.getEmval());
        }

        static auto fromWireType(WireType v) {
            return T(BindingType<emscripten::val>::fromWireType(v));
        }
    };

    template<typename T>
    struct TypeID<T, std::enable_if_t<tc::js::IsJsIntegralEnum<tc::remove_cvref_t<T>>::value>> {
        static constexpr TYPEID get() {
            return TypeID<std::underlying_type_t<tc::remove_cvref_t<T>>>::get();
        }
    };

    template<typename T>
    struct BindingType<T, std::enable_if_t<tc::js::IsJsIntegralEnum<T>::value>> {
        using UnderlyingType = std::underlying_type_t<T>;
        typedef typename BindingType<UnderlyingType>::WireType WireType;

        static WireType toWireType(T const& en) {
            return BindingType<UnderlyingType>::toWireType(static_cast<UnderlyingType>(en));
        }

        static auto fromWireType(WireType wire) {
            return static_cast<T>(BindingType<UnderlyingType>::fromWireType(wire));
        }
    };
}
