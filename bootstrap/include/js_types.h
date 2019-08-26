#pragma once
#include <type_traits>
#include <optional>
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
struct IsJsIntegralEnum : std::false_type {};

struct js_unknown {
    js_unknown(emscripten::val const& _emval) : m_emval(_emval) {}
    js_unknown(emscripten::val&& _emval) : m_emval(tc_move(_emval)) {}

    emscripten::val getEmval() const& { return m_emval; }
    emscripten::val&& getEmval() && { return tc_move(m_emval); }

    template<typename T, typename = std::enable_if_t<IsJsInteropable<tc::remove_cvref_t<T>>::value>>
    js_unknown(T&& value) : m_emval(value) {
    }

    template<typename U, typename = std::enable_if_t<IsJsInteropable<tc::remove_cvref_t<U>>::value>>
    explicit operator U() const& { return m_emval; }

    template<typename U, typename = std::enable_if_t<IsJsInteropable<tc::remove_cvref_t<U>>::value>>
    explicit operator U&&() && { return tc_move(m_emval); }

    explicit operator bool() const {
        return !!m_emval;
    }

private:
	emscripten::val m_emval;
};

struct js_string final {
    explicit js_string(emscripten::val const& _emval) noexcept : m_emval(_emval) {
        _ASSERT(m_emval.isString());
    }
    explicit js_string(emscripten::val&& _emval) noexcept : m_emval(tc_move(_emval)) {
        _ASSERT(m_emval.isString());
    }

    explicit js_string(js_unknown const& js) noexcept : js_string(js.getEmval()) {
    }
    explicit js_string(js_unknown&& js) noexcept : js_string(tc_move(js).getEmval()) {
    }

    emscripten::val getEmval() const& noexcept { return m_emval; }
    emscripten::val getEmval() && noexcept { return tc_move(m_emval); }

    template<typename Rng, typename = std::enable_if_t<
        !std::is_same<emscripten::val, tc::remove_cvref_t<Rng>>::value &&
        !std::is_same<js_unknown, tc::remove_cvref_t<Rng>>::value
    >>
    explicit js_string(Rng&& rng) noexcept : m_emval(
        // TODO: avoid allocating std::string (we may have to duplicate parts of embind)
        tc::explicit_cast<std::string>(std::forward<Rng>(rng))
    ) {
    }

    int length() { return m_emval["length"].as<int>(); }

    explicit operator std::string() { return m_emval.template as<std::string>(); }

private:
    emscripten::val m_emval;
};
} // namespace no_adl
using no_adl::IsJsInteropable;
using no_adl::IsJsIntegralEnum;
using no_adl::js_unknown;
using no_adl::js_string;

template<>
struct IsJsInteropable<js_unknown> : std::true_type {};

template<>
struct IsJsInteropable<js_string> : std::true_type {};

template<typename T>
struct IsJsInteropable<T, std::enable_if_t<IsJsIntegralEnum<T>::value>> : std::true_type {};

namespace emscripten_interop_detail {
template<typename T>
using IsEmvalWrapper = tc::type::find_unique<tc::type::list<js_unknown, js_string>, T>;
} // namespace emscripten_interop_detail

namespace optional_detail {
template<typename T>
using IsOptionalOfJsInteropable = std::bool_constant<
    tc::is_instance<std::optional, T>::value &&
    IsJsInteropable<typename T::value_type>::value &&
    !tc::is_instance<std::optional, typename T::value_type>::value
>;
} // namespace optional_detail

template<typename T>
struct IsJsInteropable<T, std::enable_if_t<tc::js::optional_detail::IsOptionalOfJsInteropable<T>::value>> : std::true_type {};
} // namespace tc::js

// Custom marshalling
namespace emscripten::internal {
    template<typename T>
    struct TypeID<T, std::enable_if_t<tc::js::emscripten_interop_detail::IsEmvalWrapper<tc::remove_cvref_t<T>>::found>> {
        static constexpr TYPEID get() {
            return TypeID<val>::get();
        }
    };

    template<typename T>
    struct BindingType<T, std::enable_if_t<tc::js::emscripten_interop_detail::IsEmvalWrapper<T>::found>> {
        typedef typename BindingType<emscripten::val>::WireType WireType;

        static WireType toWireType(T const& js) {
            return BindingType<emscripten::val>::toWireType(js.getEmval());
        }

        static auto fromWireType(WireType v) {
            return T(BindingType<emscripten::val>::fromWireType(v));
        }
    };

    // TODO: optimize by providing JS-side toWireType/fromWireType for integrals/bools and getting rid of emscripten::val
    template<typename T>
    struct TypeID<T, std::enable_if_t<tc::js::optional_detail::IsOptionalOfJsInteropable<tc::remove_cvref_t<T>>::value>> {
        static constexpr TYPEID get() {
            // Ensure that the underlying type can be marshalled.
            TypeID<typename tc::remove_cvref_t<T>::value_type>::get();
            return TypeID<val>::get();
        }
    };

    template<typename T>
    struct BindingType<T, std::enable_if_t<tc::js::optional_detail::IsOptionalOfJsInteropable<T>::value>> {
        typedef typename BindingType<emscripten::val>::WireType WireType;

        static WireType toWireType(T const& optv) {
            if (optv) {
                emscripten::val emval(optv.value());
                _ASSERT(!emval.isUndefined());
                return BindingType<emscripten::val>::toWireType(emval);
            } else {
                return BindingType<emscripten::val>::toWireType(emscripten::val::undefined());
            }
        }

        static auto fromWireType(WireType wire) {
            auto emval = BindingType<emscripten::val>::fromWireType(wire);
            T result;
            if (!emval.isUndefined()) {
                result.emplace(tc_move(emval).as<typename T::value_type>());
            }
            return result;
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
