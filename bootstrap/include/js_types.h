#pragma once
#include <emscripten/val.h>
#include <emscripten/wire.h>
#include <string>
#include <type_traits>
#include <utility>
#include "explicit_cast.h"
#include "tag_type.h"
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

    emscripten::val const& getEmval() const& noexcept { return m_emval; }
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
    emscripten::val getEmval() const noexcept { return emscripten::val::undefined(); }
};

struct js_null {
    js_null() noexcept {}
    explicit js_null(emscripten::val const& emval) noexcept {
        _ASSERT(emval.isNull());
    }
    emscripten::val getEmval() const noexcept { return emscripten::val::null(); }
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
template<typename... Ts>
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

template<typename> struct IsExplicitCastableFrom {};

template<typename... From>
struct IsExplicitCastableFrom<tc::type::list<From...>> {
    template<typename To>
    using type = tc::is_explicit_castable<To, From...>;
};
} // namespace no_adl
using no_adl::CFindValueType;
using no_adl::IsExplicitCastableFrom;

template<typename ListFrom, typename ListTs>
using FindUniqueExplicitCastableFrom = tc::type::find_unique_if<ListTs, no_adl::IsExplicitCastableFrom<ListFrom>::template type>;
} // namespace js_union_detail

namespace no_adl {
// TODO: optimize by providing JS-side toWireType/fromWireType for integrals/bools and getting rid of emscripten::val
template<typename... Ts>
struct js_union : js_union_detail::CFindValueType<Ts...> {
    static inline constexpr auto instantiated = true;

    static_assert(1 < sizeof...(Ts));
    static_assert((IsJsInteropable<Ts>::value && ...));
    static_assert((!std::is_same<Ts, js_unknown>::value && ...));

    using ListTs = tc::type::list<Ts...>;

    static inline constexpr auto has_undefined = tc::type::find_unique<ListTs, js_undefined>::found;
    static inline constexpr auto has_null = tc::type::find_unique<ListTs, js_null>::found;
    static inline constexpr auto has_string = tc::type::find_unique<ListTs, js_string>::found;
    static inline constexpr auto has_bool = tc::type::find_unique<ListTs, bool>::found;
    static inline constexpr auto has_number = tc::type::find_unique<ListTs, double>::found;

    explicit js_union(emscripten::val const& _emval) noexcept : m_emval(_emval) { assertEmvalInRange(); }
    explicit js_union(emscripten::val&& _emval) noexcept : m_emval(tc_move(_emval)) { assertEmvalInRange(); }

    emscripten::val const& getEmval() const& noexcept { return m_emval; }
    emscripten::val&& getEmval() && noexcept { return tc_move(m_emval); }

    // Constructing from a subtype. Assumption: the underlying representation does not depend on what element of ListTs is chosen.
    template<typename T, std::enable_if_t<
        IsJsInteropable<tc::remove_cvref_t<T>>::value &&
        tc::type::any_of<ListTs, tc::type::curry<std::is_convertible, T&&>::template type>::value
    >* = nullptr>
    js_union(T&& value) noexcept : m_emval(std::forward<T>(value)) {}

    // Constructing from a wider js_union (downcast).
    template<typename T, std::enable_if_t<std::conjunction<
        std::negation<std::is_same<tc::remove_cvref_t<T>, js_union>>,
        tc::is_instance_or_derived<js_union, T>,
        std::is_convertible<js_union, tc::remove_cvref_t<T>>
    >::value>* = nullptr>
    explicit js_union(T&& other) noexcept : m_emval(std::forward<T>(other).m_emval) {
        assertEmvalInRange();
    }

    template<typename... Args>
    explicit js_union(tc::aggregate_tag_t, Args&&... args) noexcept : m_emval(
        tc::explicit_cast<typename js_union_detail::FindUniqueExplicitCastableFrom<tc::type::list<Args&&...>, ListTs>::type>(
            std::forward<Args>(args)...
        )
    ) {}

    template<typename T = js_union, std::enable_if_t<T::has_undefined && !T::has_null>* = nullptr>
    js_union() noexcept : js_union(emscripten::val::undefined()) {}

    template<typename T = js_union, std::enable_if_t<!T::has_undefined && T::has_null>* = nullptr>
    js_union() noexcept : js_union(emscripten::val::null()) {}

    // Upcast to a common type. Assumption: the underlying representation does not depend on what element of ListTs is chosen.
    template<typename T, std::enable_if_t<
        !std::is_same<bool, tc::remove_cvref_t<T>>::value &&
        (std::is_convertible<Ts, T>::value && ...)
    >* = nullptr>
    operator T() const noexcept {
        return m_emval.template as<T>();
    }

    // Extracting a specific type.
    template<typename T, std::enable_if_t<
        !std::is_same<bool, tc::remove_cvref_t<T>>::value &&
        !(std::is_convertible<Ts, T>::value && ...) &&
        (std::is_same<Ts, tc::remove_cvref_t<T>>::value || ...)
    >* = nullptr>
    explicit operator T() const noexcept {
        return m_emval.template as<T>();
    }

private:
    template<typename... Us> friend struct js_union;

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

    emscripten::val const& getEmval() const& noexcept { return m_emval; }
    emscripten::val&& getEmval() && noexcept { return tc_move(m_emval); }

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

template<typename... Ts>
struct IsJsInteropable<js_union<Ts...>> : std::true_type {
    static_assert(js_union<Ts...>::instantiated);
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

template<typename... Ts>
struct IsEmvalWrapper<js_union<Ts...>, std::enable_if_t<IsJsInteropable<js_union<Ts...>>::value>> : std::true_type {};
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
            return TypeID<double>::get();
        }
    };

    template<typename T>
    struct BindingType<T, std::enable_if_t<tc::js::IsJsIntegralEnum<T>::value>> {
        using UnderlyingType = std::underlying_type_t<T>;
        static_assert(std::is_integral<UnderlyingType>::value);

        // We want rounding checks, so underlying type is 'double' directly, not 'int' (which will silently round).
        typedef typename BindingType<double>::WireType WireType;

        static WireType toWireType(T const& en) {
            return BindingType<double>::toWireType(tc::explicit_cast<double>(static_cast<UnderlyingType>(en)));
        }

        static auto fromWireType(WireType wire) {
            return static_cast<T>(tc::explicit_cast<UnderlyingType>(BindingType<double>::fromWireType(wire)));
        }
    };
}
