#pragma once
#include <emscripten/val.h>
#include <emscripten/wire.h>
#include <utility>
#include <tuple>
#include <type_traits>

#include "range_defines.h"
#include "type_traits.h"
#include "noncopyable.h"
#include "tag_type.h"
#include "tc_move.h"
#include <boost/callable_traits.hpp>

namespace tc::js {
// ---------------------------------------- Passing member functions to JS ----------------------------------------
DEFINE_TAG_TYPE(pass_this)
DEFINE_TAG_TYPE(pass_all_arguments)

namespace callback_detail {
using ArgumentPointer = void*;
using FunctionPointer = emscripten::val(*)(ArgumentPointer, emscripten::val&, emscripten::val&) noexcept;

namespace no_adl {
struct CUniqueDetachableEmscriptenVal : tc::nonmovable {
    CUniqueDetachableEmscriptenVal(FunctionPointer pFunction, ArgumentPointer pArgument) noexcept;
    ~CUniqueDetachableEmscriptenVal() { m_emval.call<void>("detach"); }

    const emscripten::val get() const { return m_emval; }
    operator const emscripten::val() const { return m_emval; }

private:
    emscripten::val m_emval;
};
} // namespace no_adl
using no_adl::CUniqueDetachableEmscriptenVal;

template<typename T, typename... Ts>
using contains_tag = std::disjunction<std::is_same<T, Ts>...>;

enum class CallableWrapperState {
    NoPassAllArguments,
    HadPassAllArguments
};

namespace no_adl {
template<typename, CallableWrapperState = CallableWrapperState::NoPassAllArguments>
struct CallableWrapper final {};

template<typename R, typename... Args, CallableWrapperState State>
struct CallableWrapper<R(Args...), State> final {
    static_assert(!contains_tag<pass_this_t, Args...>::value);
    static_assert(!contains_tag<pass_all_arguments_t, Args...>::value);

    template<typename Fn>
    emscripten::val operator()(Fn&& fn, emscripten::val& /*emvalThis*/, emscripten::val& emvalArguments) const& noexcept {
        return CallHelper<std::make_index_sequence<sizeof...(Args)>>()(std::forward<Fn>(fn), emvalArguments);
    }

private:
    template<std::size_t Index>
    static emscripten::val GetArrayElement(emscripten::val const& emvalArray) noexcept {
        return emvalArray[Index];
    }

    template<typename Fn>
    static emscripten::val CallAndWrapVal(Fn&& fn) noexcept {
        if constexpr (std::is_same<void, decltype(fn())>::value) {
            fn();
            return emscripten::val::undefined();
        } else {
            return emscripten::val(fn());
        }
    }

    template<typename>
    struct CallHelper final {};

    template<std::size_t... Indices>
    struct CallHelper<std::index_sequence<Indices...>> final {
        template<typename Fn>
        emscripten::val operator()(Fn&& fn, emscripten::val& emvalArguments) const& noexcept {
            if constexpr (State == CallableWrapperState::NoPassAllArguments) {
                _ASSERT(emvalArguments["length"].as<std::size_t>() == sizeof...(Args));
            } else if constexpr (State == CallableWrapperState::HadPassAllArguments) {
                _ASSERT(emvalArguments["length"].as<std::size_t>() >= sizeof...(Args));
            } else {
                _ASSERT(false);
            }
            return CallAndWrapVal([&]() noexcept { return fn(GetArrayElement<Indices>(emvalArguments).template as<Args>()...); });
        }
    };
};

template<typename R, typename TThis, typename... Args, CallableWrapperState State>
struct CallableWrapper<R(pass_this_t, TThis, Args...), State> final {
    static_assert(!contains_tag<pass_this_t, TThis, Args...>::value);

    template<typename Fn>
    emscripten::val operator()(Fn&& fn, emscripten::val& emvalThis, emscripten::val& emvalArguments) const& noexcept {
        return CallableWrapper<R(Args...), State>()(
            [&](auto&&... args) noexcept {
                return fn(pass_this, emvalThis.template as<TThis>(), std::forward<decltype(args)>(args)...);
            },
            emvalThis,
            emvalArguments
        );
    }
};

template<typename R, typename TArgs, typename... Args, CallableWrapperState State>
struct CallableWrapper<R(pass_all_arguments_t, TArgs, Args...), State> final {
    static_assert(!contains_tag<pass_all_arguments_t, TArgs, Args...>::value);
    static_assert(State == CallableWrapperState::NoPassAllArguments);

    template<typename Fn>
    emscripten::val operator()(Fn&& fn, emscripten::val& emvalThis, emscripten::val& emvalArguments) const& noexcept {
        return CallableWrapper<R(Args...), CallableWrapperState::HadPassAllArguments>()(
            [&](auto&&... args) noexcept {
                return fn(pass_all_arguments, emvalArguments.template as<TArgs>(), std::forward<decltype(args)>(args)...);
            },
            emvalThis,
            emvalArguments
        );
    }
};

template<typename> struct drop_first_tuple_type final {};
template<typename T, typename... Ts> struct drop_first_tuple_type<std::tuple<T, Ts...>> final {
    typedef std::tuple<Ts...> type;
};
} // namespace no_adl
using no_adl::CallableWrapper;
using no_adl::drop_first_tuple_type;

template<typename T>
emscripten::val MemberFunctionWrapper(T pmfMember, void* pThis, emscripten::val& emvalThis, emscripten::val& emvalArguments) noexcept {
    using ThisType = boost::callable_traits::class_of_t<T>;
    static_assert(boost::callable_traits::is_noexcept<T>::value, "Member functions exported to JS should be noexcept");
    static_assert(!std::is_copy_constructible_v<ThisType>, "The struct with JS-exported member functions should not have copy constructors");
    static_assert(!std::is_move_constructible_v<ThisType>, "The struct with JS-exported member functions should not have move constructors");

    return CallableWrapper<
        boost::callable_traits::apply_return_t<
            typename drop_first_tuple_type<boost::callable_traits::args_t<T>>::type,
            boost::callable_traits::return_type_t<T>
        >
    >()([pThis, pmfMember](auto&&... args) noexcept {
        return (static_cast<ThisType*>(pThis)->*pmfMember)(std::forward<decltype(args)>(args)...);
    }, emvalThis, emvalArguments);
}
} // namespace callback_detail

#define TC_WASM_UNIQUE_NAME_IMPL2(Prefix, Line, Counter) Prefix##_##Line##_##Counter
#define TC_WASM_UNIQUE_NAME_IMPL1(Prefix, Line, Counter) TC_WASM_UNIQUE_NAME_IMPL2(Prefix, Line, Counter)
#define TC_WASM_UNIQUE_NAME(Prefix) TC_WASM_UNIQUE_NAME_IMPL1(Prefix, __LINE__, __COUNTER__)

#define TC_WASM_MEMBER_FUNCTION_NAMED_WRAPPER(FieldName, WrapperName, MemberPointer) \
    static emscripten::val WrapperName(void* pThis, emscripten::val& emvalThis, emscripten::val& emvalArguments) noexcept { \
        return ::tc::js::callback_detail::MemberFunctionWrapper(MemberPointer, pThis, emvalThis, emvalArguments); \
    } \
    const ::tc::js::callback_detail::CUniqueDetachableEmscriptenVal FieldName{&WrapperName, static_cast<void*>(this)};

#define TC_WASM_MEMBER_FUNCTION_WRAPPER(FieldName, MemberPointer) \
    TC_WASM_MEMBER_FUNCTION_NAMED_WRAPPER(FieldName, TC_WASM_UNIQUE_NAME(_tc_js_member_function_wrapper), MemberPointer)

#define TC_WASM_NAMED_MEMBER_JS_FUNCTION(ClassName, FieldName, ReturnType, MethodName) \
    void TC_WASM_UNIQUE_NAME(_tc_js_member_function_type_check)() { \
        STATICASSERTSAME(ClassName&, decltype(*this), "ClassName specified in TC_WASM_NAMED_MEMBER_JS_FUNCTION is incorrect"); \
    } \
    TC_WASM_MEMBER_FUNCTION_WRAPPER(FieldName, &ClassName::MethodName) \
    ReturnType MethodName

#define TC_WASM_MEMBER_JS_FUNCTION(ClassName, FieldName, ReturnType) \
    TC_WASM_NAMED_MEMBER_JS_FUNCTION(ClassName, FieldName, ReturnType, TC_WASM_UNIQUE_NAME(_tc_js_member_function_impl))

// ---------------------------------------- Scoped/heap callbacks ----------------------------------------
template<typename T, typename... Args> constexpr auto KeepThisCallback(Args&&...) noexcept;
template<typename T, typename... Args> constexpr auto DeleteThisCallback(Args&&...) noexcept;

namespace callback_detail {
namespace no_adl {
template<typename T>
struct SCallbackResult final {
    constexpr bool ShouldKeepAlive() const& noexcept { return m_bKeepAlive; }
    constexpr T Extract() noexcept { return tc_move(m_tResult); }

    template<typename, typename... Args> friend constexpr auto tc::js::KeepThisCallback(Args&&...) noexcept;
    template<typename, typename... Args> friend constexpr auto tc::js::DeleteThisCallback(Args&&...) noexcept;

private:
    template<typename... Args>
    constexpr SCallbackResult(bool m_bKeepAlive, Args&&... m_tResult) noexcept
        : m_tResult(std::forward<Args>(m_tResult)...)
        , m_bKeepAlive(m_bKeepAlive) {}

    T m_tResult;
    bool m_bKeepAlive;
};

template<>
struct SCallbackResult<void> final {
    constexpr bool ShouldKeepAlive() const& noexcept { return m_bKeepAlive; }
    constexpr void Extract() noexcept {};

    template<typename, typename... Args> friend constexpr auto tc::js::KeepThisCallback(Args&&...) noexcept;
    template<typename, typename... Args> friend constexpr auto tc::js::DeleteThisCallback(Args&&...) noexcept;

private:
    constexpr SCallbackResult(bool m_bKeepAlive) noexcept
        : m_bKeepAlive(m_bKeepAlive) {}

    bool m_bKeepAlive;
};
template<typename T> struct IsSCallbackResult final : std::false_type {};
template<typename T> struct IsSCallbackResult<SCallbackResult<T>> final : std::true_type {};
} // namespace no_adl
using no_adl::SCallbackResult;
using no_adl::IsSCallbackResult;
} // namespace callback_detail

namespace no_adl {
template<typename Fn>
struct CScopedCallback final : tc::nonmovable {
    static_assert(!std::is_reference<Fn>::value);
    explicit CScopedCallback(Fn&& fn) noexcept : m_fn(tc_move(fn)) {}

private:
    Fn m_fn;
    TC_WASM_MEMBER_JS_FUNCTION(CScopedCallback, m_emvalCall, emscripten::val)(tc::js::pass_this_t, emscripten::val emvalThis, tc::js::pass_all_arguments_t, emscripten::val emvalArguments) noexcept {
        static_assert(
            !callback_detail::IsSCallbackResult<boost::callable_traits::return_type_t<Fn>>::value,
            "CScopedCallback cannot return SCallbackResult. If you want callback to self-manage lifetime, use NewHeapCallback"
        );
        static_assert(boost::callable_traits::is_noexcept<Fn>::value, "Callbacks for JS should be noexcept");
        return callback_detail::CallableWrapper<boost::callable_traits::function_type_t<Fn>>()(m_fn, emvalThis, emvalArguments);
    }

    friend emscripten::internal::BindingType<CScopedCallback>;
};
template<typename Fn> CScopedCallback(Fn&&) -> CScopedCallback<tc::decay_t<Fn>>;
} // namespace no_adl
using no_adl::CScopedCallback;

namespace callback_detail {
namespace no_adl {
template<typename T> struct IsCScopedCallback : std::false_type {};
template<typename Fn> struct IsCScopedCallback<tc::js::CScopedCallback<Fn>> : std::true_type {};
} // namespace no_adl
using no_adl::IsCScopedCallback;
} // namespace callback_detail

template<typename Fn> emscripten::val NewHeapCallback(Fn&& fn) noexcept;

namespace callback_detail {
namespace no_adl {
template<typename Fn>
struct CHeapCallback final : tc::nonmovable {
private:
    static_assert(!std::is_reference<Fn>::value);
    explicit CHeapCallback(Fn&& fn) noexcept : m_fn(tc_move(fn)) {}

    friend emscripten::val tc::js::NewHeapCallback(Fn&& fn) noexcept;

    Fn m_fn;
    TC_WASM_MEMBER_JS_FUNCTION(CHeapCallback, m_emvalCall, emscripten::val)(tc::js::pass_this_t, emscripten::val emvalThis, tc::js::pass_all_arguments_t, emscripten::val emvalArguments) noexcept {
        static_assert(
            callback_detail::IsSCallbackResult<boost::callable_traits::return_type_t<Fn>>::value,
            "CHeapCallback's functor should return SCallbackResult<> to signal destruction. Alternatively, use CScopedCallback"
        );
        static_assert(boost::callable_traits::is_noexcept<Fn>::value, "Callbacks for JS should be noexcept");
        return CallableWrapper<boost::callable_traits::function_type_t<Fn>>()([&](auto&&... args) noexcept {
            auto result = m_fn(std::forward<decltype(args)>(args)...);
            if (!result.ShouldKeepAlive()) {
                delete this;
            }
            return result.Extract();
        }, emvalThis, emvalArguments);
    }
};
} // namespace no_adl
using no_adl::CHeapCallback;
} // namespace callback_detail

template<typename T, typename... Args> constexpr auto KeepThisCallback(Args&&... args) noexcept {
    return callback_detail::SCallbackResult<tc::decay_t<T>>(true, std::forward<Args>(args)...);
}

template<typename T, typename... Args> constexpr auto DeleteThisCallback(Args&&... args) noexcept {
    return callback_detail::SCallbackResult<tc::decay_t<T>>(false, std::forward<Args>(args)...);
}

template<typename T> constexpr auto KeepThisCallback(T&& tResult) noexcept {
    return KeepThisCallback<tc::decay_t<T>, T>(std::forward<T>(tResult));
}

template<typename T> constexpr auto DeleteThisCallback(T&& tResult) noexcept {
    return DeleteThisCallback<tc::decay_t<T>, T>(std::forward<T>(tResult));
}

inline constexpr auto KeepThisCallback() noexcept {
    return KeepThisCallback<void>();
}

inline constexpr auto DeleteThisCallback() noexcept {
    return DeleteThisCallback<void>();
}

template<typename Fn>
emscripten::val NewHeapCallback(Fn&& fn) noexcept { // TODO: even std::bad_alloc?
    return (new callback_detail::CHeapCallback<tc::decay_t<Fn>>(tc_move(fn)))->m_emvalCall;
}

// ---------------------------------------- Fires-once callback ----------------------------------------
namespace fires_once_callback_detail {
namespace no_adl {
template<typename, typename, typename>
struct FiresOnceCallbackWrapper final {};

template<typename R, typename Fn, typename... Args>
struct FiresOnceCallbackWrapper<R, Fn, std::tuple<Args...>> final {
    template<typename U>
    constexpr FiresOnceCallbackWrapper(U&& m_fn) noexcept : m_fn(std::forward<U>(m_fn)) {}

    // `Args` is the exact arguments of underlying lambda that we have to mimic.
    // We do not want `int` to become `int&` or `int&&` here and change signature,
    // but we still want perfect forwarding, hence `forward<>` without forwarding
    // reference.
    constexpr callback_detail::SCallbackResult<R> operator()(Args... args) noexcept {
        if constexpr (std::is_same<void, R>::value) {
            m_fn(std::forward<Args>(args)...);
            return DeleteThisCallback();
        } else {
            return DeleteThisCallback(m_fn(std::forward<Args>(args)...));
        }
    }
private:
    Fn m_fn;
};
}
using no_adl::FiresOnceCallbackWrapper;
} // namespace new_fires_once_callback_details

template<typename Fn>
emscripten::val NewFiresOnceCallback(Fn&& fn) noexcept { // TODO: even std::bad_alloc?
    return NewHeapCallback(fires_once_callback_detail::FiresOnceCallbackWrapper<
        boost::callable_traits::return_type_t<Fn>,
        Fn,
        boost::callable_traits::args_t<Fn>
        >(std::forward<Fn>(fn)));
}
} // namespace tc::js

// ---------------------------------------- Custom embind marshalling ----------------------------------------
namespace emscripten::internal {
	template<typename Fn>
	struct BindingType<tc::js::CScopedCallback<Fn>> {
		typedef internal::EM_VAL WireType;
		static WireType toWireType(tc::js::CScopedCallback<Fn> const& cbl) {
			return BindingType<val>::toWireType(cbl.m_emvalCall);
		}
	};

	template<typename T>
	struct TypeID<T, std::enable_if_t<tc::js::callback_detail::IsCScopedCallback<tc::remove_cvref_t<T>>::value>> {
		static constexpr TYPEID get() {
			return LightTypeID<val>::get();
		}
	};

	template<>
	struct BindingType<tc::js::callback_detail::CUniqueDetachableEmscriptenVal> {
		typedef internal::EM_VAL WireType;
		static WireType toWireType(tc::js::callback_detail::CUniqueDetachableEmscriptenVal const& v) {
			return BindingType<val>::toWireType(v.get());
		}
	};

	template<typename T>
	struct TypeID<T, std::enable_if_t<
	    std::is_same<
	        tc::js::callback_detail::CUniqueDetachableEmscriptenVal,
	        tc::remove_cvref_t<T>
	    >::value
	>> {
		static constexpr TYPEID get() {
			return LightTypeID<val>::get();
		}
	};
}
