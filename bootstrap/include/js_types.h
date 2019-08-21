#pragma once
#include <type_traits>
#include <optional>
#include <emscripten/val.h>
#include <emscripten/wire.h>
#include "noncopyable.h"
#include "type_list.h"
#include "type_traits.h"
#include "tc_move.h"
#include "js_ref.h"

namespace tc::js {
namespace no_adl {
struct IUnknown : virtual IJsBase {
};

template<typename T>
struct IsJsIntegralEnum : std::false_type {};
} // namespace no_adl
using no_adl::IUnknown;
using no_adl::IsJsIntegralEnum;

template<typename T>
struct IsJsInteropable<T, std::enable_if_t<IsJsIntegralEnum<T>::value>> : std::true_type {};

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
