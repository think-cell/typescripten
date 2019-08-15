#pragma once
#include <emscripten/val.h>
#include <emscripten/wire.h>
#include <cstdint>
#include <memory>
#include <utility>
#include <tuple>
#include <type_traits>

#include "casts.h"
#include "range_defines.h"
#include "type_traits.h"
#include "noncopyable.h"
#include "tag_type.h"
#include "tc_move.h"
#include "type_list.h"
#include <boost/callable_traits.hpp>

namespace tc::js {
// ---------------------------------------- Passing member functions to JS ----------------------------------------
DEFINE_TAG_TYPE(pass_this)
DEFINE_TAG_TYPE(pass_all_arguments)

namespace callback_detail {
namespace no_adl {
template<typename lArgs, bool bPassedAllArguments = false>
struct CCallableWrapper final {
    static_assert(!tc::type::contains<lArgs, pass_this_t>::value);
    static_assert(!tc::type::contains<lArgs, pass_all_arguments_t>::value);
    static_assert(!tc::type::any_of<lArgs, std::is_reference>::value);

    template<typename Fn>
    emscripten::val operator()(Fn&& fn, emscripten::val const& /*emvalThis*/, emscripten::val const& emvalArgs) const& noexcept {
        if constexpr (bPassedAllArguments) {
            _ASSERT(tc::type::size<lArgs>::value <= emvalArgs["length"].as<std::size_t>());
        } else {
            _ASSERTEQUAL(tc::type::size<lArgs>::value, emvalArgs["length"].as<std::size_t>());
        }
        return CCallHelper<lArgs, std::make_index_sequence<tc::type::size<lArgs>::value>>()(std::forward<Fn>(fn), emvalArgs);
    }

private:
    template<typename, typename>
    struct CCallHelper final {};

    template<typename... Args, std::size_t... Indices>
    struct CCallHelper<tc::type::list<Args...>, std::index_sequence<Indices...>> final {
        template<typename Fn>
        emscripten::val operator()(Fn&& fn, emscripten::val const& emvalArgs) const& noexcept {
            auto fnWithArgs = [&]() noexcept { return fn(emvalArgs[Indices].template as<Args>()...); };
            using ReturnType = decltype(fnWithArgs());
            static_assert(!std::is_reference<ReturnType>::value);
            if constexpr (std::is_same<void, ReturnType>::value) {
                fnWithArgs();
                return emscripten::val::undefined();
            } else {
                return emscripten::val(fnWithArgs());
            }
        }
    };
};

template<typename TThis, typename... ArgsTail, bool bPassedAllArguments>
struct CCallableWrapper<tc::type::list<pass_this_t, TThis, ArgsTail...>, bPassedAllArguments> final {
private:
    using lArgsTail = tc::type::list<ArgsTail...>;
public:
    static_assert(!tc::type::contains<lArgsTail, pass_this_t>::value);
    static_assert(!std::is_reference<TThis>::value);

    template<typename Fn>
    emscripten::val operator()(Fn&& fn, emscripten::val const& emvalThis, emscripten::val const& emvalArgs) const& noexcept {
        return CCallableWrapper<lArgsTail, bPassedAllArguments>()(
            [&](auto&&... args) noexcept {
                return fn(pass_this, emvalThis.template as<TThis>(), std::forward<decltype(args)>(args)...);
            },
            emvalThis,
            emvalArgs
        );
    }
};

template<typename TArgs, typename... ArgsTail, bool bPassedAllArguments>
struct CCallableWrapper<tc::type::list<pass_all_arguments_t, TArgs, ArgsTail...>, bPassedAllArguments> final {
private:
    using lArgsTail = tc::type::list<ArgsTail...>;
public:
    static_assert(!tc::type::contains<lArgsTail, pass_all_arguments_t>::value);
    static_assert(!std::is_reference<TArgs>::value);
    static_assert(!bPassedAllArguments);

    template<typename Fn>
    emscripten::val operator()(Fn&& fn, emscripten::val const& emvalThis, emscripten::val const& emvalArgs) const& noexcept {
        return CCallableWrapper<lArgsTail, /*bPassedAllArguments=*/true>()(
            [&](auto&&... args) noexcept {
                return fn(pass_all_arguments, emvalArgs.template as<TArgs>(), std::forward<decltype(args)>(args)...);
            },
            emvalThis,
            emvalArgs
        );
    }
};
} // namespace no_adl
using no_adl::CCallableWrapper;

template<typename Pmf, typename = tc::void_t<boost::callable_traits::class_of_t<Pmf>>>
using MemberFunctionExpectedArgs_t =
    tc::type::drop_first_t<boost::callable_traits::args_t<Pmf, tc::type::list>>;

template<typename Pmf>
using MemberFunctionJsSignature_t =
    boost::callable_traits::apply_return<
        tc::type::apply<std::tuple, MemberFunctionExpectedArgs_t<Pmf>>,
        boost::callable_traits::return_type<Pmf>
    >;

template<typename T>
emscripten::val MemberFunctionWrapper(T pmfMember, void* pvThis, emscripten::val const& emvalThis, emscripten::val const& emvalArgs) noexcept {
    using ThisType = boost::callable_traits::class_of_t<T>;
    static_assert(boost::callable_traits::is_noexcept<T>::value, "Member functions exported to JS should be noexcept");
    static_assert(!std::is_copy_constructible<ThisType>::value, "The struct with JS-exported member functions should not have copy constructors");
    static_assert(!std::is_move_constructible<ThisType>::value, "The struct with JS-exported member functions should not have move constructors");

    return CCallableWrapper<MemberFunctionExpectedArgs_t<T>>()(
        [&](auto&&... args) noexcept {
            return (tc::void_cast<ThisType>(pvThis)->*pmfMember)(std::forward<decltype(args)>(args)...);
        }, emvalThis, emvalArgs
    );
}

using FirstArgument = void*;
using FunctionPointer = emscripten::val(*)(FirstArgument, emscripten::val const& emvalThis, emscripten::val const& emvalArgs) noexcept;
using PointerNumber = std::uintptr_t;

namespace no_adl {
// We do not care about slicing to emscripten::val, because this class is
// only stored as a by-value field.
struct CUniqueDetachableEmscriptenVal : tc::nonmovable, emscripten::val {
    CUniqueDetachableEmscriptenVal(FunctionPointer pfunc, FirstArgument arg0) noexcept : emscripten::val(emscripten::val::undefined()) {
        // See comments about correctness in js_callback.cpp.
        _ASSERT(std::get_pointer_safety() == std::pointer_safety::preferred || std::get_pointer_safety() == std::pointer_safety::relaxed);
        emscripten::val::operator=(emscripten::val::module_property("tc_js_callback_detail_js_CreateJsFunction")(reinterpret_cast<PointerNumber>(pfunc), reinterpret_cast<PointerNumber>(arg0)));
    }

    ~CUniqueDetachableEmscriptenVal() {
        call<void>("detach");
    }
};
} // namespace no_adl
using no_adl::CUniqueDetachableEmscriptenVal;
} // namespace callback_detail

#define TC_JS_UNIQUE_NAME_IMPL2(Prefix, Line, Counter) Prefix##_##Line##_##Counter
#define TC_JS_UNIQUE_NAME_IMPL1(Prefix, Line, Counter) TC_JS_UNIQUE_NAME_IMPL2(Prefix, Line, Counter)
#define TC_JS_UNIQUE_NAME(Prefix) TC_JS_UNIQUE_NAME_IMPL1(Prefix, __LINE__, __COUNTER__)

#define TC_JS_MEMBER_FUNCTION_NAMED_WRAPPER(FieldName, WrapperName, MemberPointer) \
    static emscripten::val WrapperName(void* pvThis, emscripten::val const& emvalThis, emscripten::val const& emvalArgs) noexcept { \
        return ::tc::js::callback_detail::MemberFunctionWrapper(MemberPointer, pvThis, emvalThis, emvalArgs); \
    } \
    const ::tc::js::callback_detail::CUniqueDetachableEmscriptenVal FieldName{&WrapperName, this};

#define TC_JS_MEMBER_FUNCTION_WRAPPER(FieldName, MemberPointer) \
    TC_JS_MEMBER_FUNCTION_NAMED_WRAPPER(FieldName, TC_JS_UNIQUE_NAME(_tc_js_member_function_wrapper), MemberPointer)

#define TC_JS_NAMED_MEMBER_FUNCTION(ClassName, FieldName, ReturnType, MethodName, Arguments) \
    void TC_JS_UNIQUE_NAME(_tc_js_member_function_type_check)() { \
        STATICASSERTSAME(ClassName&, decltype(*this), "ClassName specified in TC_JS_NAMED_MEMBER_FUNCTION is incorrect"); \
    } \
    TC_JS_MEMBER_FUNCTION_WRAPPER(FieldName, &ClassName::MethodName) \
    ReturnType MethodName Arguments noexcept

#define TC_JS_MEMBER_FUNCTION(ClassName, FieldName, ReturnType, Arguments) \
    TC_JS_NAMED_MEMBER_FUNCTION(ClassName, FieldName, ReturnType, TC_JS_UNIQUE_NAME(_tc_js_member_function_impl), Arguments)

// ---------------------------------------- Scoped/heap callbacks ----------------------------------------
template<typename T, typename... Args> constexpr auto KeepThisCallback(Args&&...) noexcept;
template<typename T, typename... Args> constexpr auto DeleteThisCallback(Args&&...) noexcept;

namespace callback_detail {
namespace no_adl {
template<typename T>
struct SCallbackResult final {
    constexpr bool ShouldKeepAlive() const& noexcept { return m_bKeepAlive; }
    constexpr T Extract() noexcept { return tc_move(m_tResult); }

    using value_type = T;

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

    using value_type = void;

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
// We do not care about slicing to emscripten::val, because this class is only stored by-value.
template<typename Fn>
struct CScopedCallback final : callback_detail::CUniqueDetachableEmscriptenVal {
    static_assert(!std::is_reference<Fn>::value);
    static_assert(
        !callback_detail::IsSCallbackResult<boost::callable_traits::return_type_t<Fn>>::value,
        "CScopedCallback cannot return SCallbackResult. If you want callback to self-manage lifetime, use NewHeapCallback"
    );
    static_assert(boost::callable_traits::is_noexcept<Fn>::value, "Callbacks for JS should be noexcept");

    template<typename FnSrc>
    CScopedCallback(FnSrc&& fn) noexcept : CUniqueDetachableEmscriptenVal(&FnWrapper, this), m_fn(std::forward<FnSrc>(fn)) {}

private:
    Fn m_fn;

    static emscripten::val FnWrapper(void* pThis, emscripten::val const& emvalThis, emscripten::val const& emvalArgs) noexcept {
        return callback_detail::CCallableWrapper<boost::callable_traits::args_t<Fn, tc::type::list>>()(
            tc::void_cast<CScopedCallback>(pThis)->m_fn,
            emvalThis,
            emvalArgs
        );
    }
};
template<typename Fn> CScopedCallback(Fn) -> CScopedCallback<Fn>;
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
// We do not care about slicing to emscripten::val, because this class is only
// created by NewHeapCallback and deleted by itself.
template<typename Fn>
struct CHeapCallback final : CUniqueDetachableEmscriptenVal {
private:
    static_assert(!std::is_reference<Fn>::value);
    static_assert(
        callback_detail::IsSCallbackResult<boost::callable_traits::return_type_t<Fn>>::value,
        "CHeapCallback's functor should return SCallbackResult<> to signal destruction. Alternatively, use CScopedCallback"
    );
    static_assert(boost::callable_traits::is_noexcept<Fn>::value, "Callbacks for JS should be noexcept");

    template<typename FnSrc>
    explicit CHeapCallback(FnSrc&& fn) noexcept : CUniqueDetachableEmscriptenVal(&FnWrapper, this), m_fn(std::forward<FnSrc>(fn)) {}

    friend emscripten::val tc::js::NewHeapCallback(Fn&& fn) noexcept;

    Fn m_fn;
    static emscripten::val FnWrapper(void* pvThis, emscripten::val const& emvalThis, emscripten::val const& emvalArgs) noexcept {
        return CCallableWrapper<boost::callable_traits::args_t<Fn, tc::type::list>>()([&](auto&&... args) noexcept {
            auto pcbThis = tc::void_cast<CHeapCallback>(pvThis);
            auto result = pcbThis->m_fn(std::forward<decltype(args)>(args)...);
            if (!result.ShouldKeepAlive()) {
                delete pcbThis;
            }
            return result.Extract();
        }, emvalThis, emvalArgs);
    }
};
template<typename Fn> CHeapCallback(Fn) -> CHeapCallback<Fn>;
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
    return *(new callback_detail::CHeapCallback(std::forward<Fn>(fn)));
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
// Even though CScopedCallback<> and CUniqueDetachableEmscriptenVal are subclasses of emscripten::val,
// template specializations and embind's bindings are invariant, so we have to provide our own.
namespace emscripten::internal {
	template<typename Fn>
	struct BindingType<tc::js::CScopedCallback<Fn>> {
		typedef BindingType<val>::WireType WireType;
		static WireType toWireType(tc::js::CScopedCallback<Fn> const& cbl) {
			return BindingType<val>::toWireType(cbl);
		}
	};

	template<typename T>
	struct TypeID<T, std::enable_if_t<tc::js::callback_detail::IsCScopedCallback<tc::remove_cvref_t<T>>::value>> {
		static constexpr TYPEID get() {
			return TypeID<val>::get();
		}
	};

	template<>
	struct BindingType<tc::js::callback_detail::CUniqueDetachableEmscriptenVal> {
		typedef BindingType<val>::WireType WireType;
		static WireType toWireType(tc::js::callback_detail::CUniqueDetachableEmscriptenVal const& v) {
			return BindingType<val>::toWireType(v);
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
			return TypeID<val>::get();
		}
	};
}
