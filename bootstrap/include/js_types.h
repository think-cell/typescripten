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
namespace wrapper_detail {
namespace no_adl {
template<typename T, typename Name>
struct CPropertyProxy : tc::nonmovable {
    CPropertyProxy(emscripten::val m_emval, Name m_name)
        : m_emval(tc_move(m_emval))
        , m_name(tc_move(m_name)) {
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
    emscripten::val m_emval;
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

namespace types_detail {
namespace no_adl {
using NativeBindingTypes = tc::type::list<
    char,
    signed char,
    unsigned char,
    signed short,
    unsigned short,
    signed int,
    unsigned int,
    signed long,
    unsigned long,
    float,
    double
>;
} // namespace no_adl
using no_adl::NativeBindingTypes;

template<typename T, typename = void>
struct IsNativeBindingOptional : std::false_type {};

template<typename T>
struct IsNativeBindingOptional<std::optional<T>, std::enable_if_t<tc::type::has_unique<NativeBindingTypes, T>::value>> : std::true_type {};
} // namespace types_detail
} // namespace tc::js

// Custom marshalling
namespace emscripten::internal {
    // TODO: optimize by providing JS-side toWireType/fromWireType and getting rid of emscripten::val
    template<typename T>
    struct TypeID<T, std::enable_if_t<tc::js::types_detail::IsNativeBindingOptional<tc::remove_cvref_t<T>>::value>> {
        static constexpr TYPEID get() {
            return TypeID<val>::get();
        }
    };

    template<typename T>
    struct BindingType<T, std::enable_if_t<tc::js::types_detail::IsNativeBindingOptional<T>::value>> {
        typedef typename BindingType<emscripten::val>::WireType WireType;

        static WireType toWireType(T const& opti) {
            if (opti)
                return BindingType<emscripten::val>::toWireType(emscripten::val(opti.value()));
            else
                return BindingType<emscripten::val>::toWireType(emscripten::val::undefined());
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
}
