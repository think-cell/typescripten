#pragma once
#include <emscripten/val.h>
#include <emscripten/wire.h>
#include <string>
#include <type_traits>
#include <utility>
#include "explicit_cast.h"
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
            double,
            bool
        >,
        T
    >::found>
> : std::true_type {};

template<typename T>
struct IsJsIntegralEnum : std::false_type {};

struct js_unknown {
    explicit js_unknown(emscripten::val const& _emval) noexcept : m_emval(_emval) {}
    explicit js_unknown(emscripten::val&& _emval) noexcept : m_emval(tc_move(_emval)) {}

    emscripten::val getEmval() const& noexcept { return m_emval; }
    emscripten::val&& getEmval() && noexcept { return tc_move(m_emval); }

    template<typename T, typename = std::enable_if_t<IsJsInteropable<tc::remove_cvref_t<T>>::value>>
    js_unknown(T&& value) noexcept : m_emval(std::forward<T>(value)) {
    }

    template<typename U, typename = std::enable_if_t<IsJsInteropable<tc::remove_cvref_t<U>>::value>>
    explicit operator U() const& noexcept { return m_emval.template as<U>(); }

    explicit operator bool() const noexcept {
        return !!m_emval;
    }

private:
	emscripten::val m_emval;
};

struct js_undefined {
    js_undefined() noexcept {}
    explicit js_undefined(emscripten::val const& emval) noexcept {
        _ASSERT(emval.isUndefined());
    }
    emscripten::val getEmval() const& noexcept { return emscripten::val::undefined(); }
};

struct js_null {
    js_null() noexcept {}
    explicit js_null(emscripten::val const& emval) noexcept {
        _ASSERT(emval.isNull());
    }
    emscripten::val getEmval() const& noexcept { return emscripten::val::null(); }
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
};

template<typename T>
struct CFindValueType<js_undefined, T> {
    using value_type = T;
};

template<typename T>
struct CFindValueType<js_null, T> {
    using value_type = T;
};

template<typename From>
struct ExplicitlyConstructibleFrom {
    template<typename To>
    using type = std::conjunction<
        std::is_constructible<To, From>,
        std::negation<std::is_convertible<From, To>>
    >;
};

template<typename From>
struct ConvertibleFrom {
    template<typename To>
    using type = std::is_convertible<From, To>;
};

template<typename To>
struct ConstructibleTo {
    template<typename From>
    using type = std::is_constructible<To, From>;
};
} // namespace no_adl
using no_adl::CFindValueType;

template<typename From, typename ListArgs>
using FindUniqueExplicitlyConstructibleFrom = tc::type::find_unique_if<ListArgs, no_adl::ExplicitlyConstructibleFrom<From>::template type>;

template<typename From, typename ListArgs>
using FindUniqueConvertibleFrom = tc::type::find_unique_if<ListArgs, no_adl::ConvertibleFrom<From>::template type>;

template<typename To, typename ListArgs>
using FindUniqueConstructibleTo = tc::type::find_unique_if<ListArgs, no_adl::ConstructibleTo<To>::template type>;

namespace no_adl {
template<typename From, typename ListArgs>
struct HasUniqueExplicitlyConstructibleFrom : std::bool_constant<FindUniqueExplicitlyConstructibleFrom<From, ListArgs>::found> {
};

template<typename To, typename ListArgs>
struct HasUniqueConstructibleTo : std::bool_constant<FindUniqueConstructibleTo<To, ListArgs>::found> {
};
}
using no_adl::HasUniqueExplicitlyConstructibleFrom;
using no_adl::HasUniqueConstructibleTo;
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
    static inline constexpr auto has_number = tc::type::find_unique<ListArgs, double>::found;

    explicit js_union(emscripten::val const& _emval) noexcept : m_emval(_emval) { assertEmvalInRange(); }
    explicit js_union(emscripten::val&& _emval) noexcept : m_emval(tc_move(_emval)) { assertEmvalInRange(); }

    emscripten::val getEmval() const& noexcept { return m_emval; }
    emscripten::val&& getEmval() && noexcept { return tc_move(m_emval); }

    template<typename T, std::enable_if_t<
        IsJsInteropable<tc::remove_cvref_t<T>>::value &&
        js_union_detail::FindUniqueConvertibleFrom<T&&, ListArgs>::found
    >* = nullptr>
    js_union(T&& value) noexcept : m_emval(
        typename js_union_detail::FindUniqueConvertibleFrom<T&&, ListArgs>::type(
            std::forward<T>(value)
        )
    ) {}

    template<typename T, std::enable_if_t<std::conjunction<
        std::negation<std::is_same<emscripten::val, tc::remove_cvref_t<T>>>,
        js_union_detail::HasUniqueExplicitlyConstructibleFrom<T&&, ListArgs>
    >::value>* = nullptr>
    explicit js_union(T&& value) noexcept : m_emval(
        typename js_union_detail::FindUniqueExplicitlyConstructibleFrom<T&&, ListArgs>::type(
            std::forward<T>(value)
        )
    ) {}

    template<typename T = js_union, std::enable_if_t<T::has_undefined && !T::has_null>* = nullptr>
    js_union() noexcept : js_union(emscripten::val::undefined()) {}

    template<typename T = js_union, std::enable_if_t<!T::has_undefined && T::has_null>* = nullptr>
    js_union() noexcept : js_union(emscripten::val::null()) {}

    template<typename T, typename = std::enable_if_t<std::conjunction<
        std::negation<std::is_same<T, tc::remove_cvref_t<bool>>>,
        js_union_detail::HasUniqueConstructibleTo<T&&, ListArgs>
    >::value>>
    explicit operator T() const noexcept {
        return T(m_emval.template as<typename js_union_detail::FindUniqueConstructibleTo<T&&, ListArgs>::type>());
    }

private:
    template<typename T>
    struct CArrowProxy {
        T m_value;
        CArrowProxy(T&& value) noexcept : m_value(tc_move(value)) {}
        T* operator->() noexcept { return &m_value; }
        T const* operator->() const noexcept { return &m_value; }
    };

public:
    template<typename T = js_union>
    typename T::value_type operator*() const noexcept {
        return operator typename js_union::value_type();
    }

    template<typename T = js_union>
    CArrowProxy<typename T::value_type> operator->() const noexcept {
        return operator typename js_union::value_type();
    }

    explicit operator bool() const noexcept {
        return !!m_emval;
    }

private:
    emscripten::val m_emval;

    void assertEmvalInRange() const noexcept {
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

    template<typename Rng, typename = std::enable_if_t<tc::is_explicit_castable<std::string, Rng&&>::value>>
    explicit js_string(Rng&& rng) noexcept : m_emval(
        // TODO: avoid allocating std::string (we may have to duplicate parts of embind)
        tc::explicit_cast<std::string>(std::forward<Rng>(rng))
    ) {
    }

    int length() noexcept { return m_emval["length"].as<int>(); }

    explicit operator bool() const noexcept {
        return !!m_emval;
    }

    explicit operator std::string() noexcept { return m_emval.template as<std::string>(); }

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
