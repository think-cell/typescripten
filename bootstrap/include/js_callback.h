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
#include "js_ref.h"

namespace tc::js {
// ---------------------------------------- Passing member functions to JS ----------------------------------------
DEFINE_TAG_TYPE(pass_this)
DEFINE_TAG_TYPE(pass_all_arguments)

namespace callback_detail {
namespace no_adl {
template<typename ListArgs, bool bPassedAllArguments = false>
struct CCallableWrapper final {
    static_assert(!tc::type::find_unique<ListArgs, pass_this_t>::found);
    static_assert(!tc::type::find_unique<ListArgs, pass_all_arguments_t>::found);
    static_assert(tc::type::all_of<ListArgs, IsJsInteropable>::value);

    static inline constexpr bool c_bInstantiated = true;

    template<typename Fn>
    emscripten::val operator()(Fn&& fn, emscripten::val const& /*emvalThis*/, emscripten::val const& emvalArgs) const& noexcept {
        if constexpr (bPassedAllArguments) {
            _ASSERT(tc::type::size<ListArgs>::value <= emvalArgs["length"].as<std::size_t>());
        } else {
            _ASSERTEQUAL(tc::type::size<ListArgs>::value, emvalArgs["length"].as<std::size_t>());
        }
        return CCallHelper<ListArgs, std::make_index_sequence<tc::type::size<ListArgs>::value>>()(std::forward<Fn>(fn), emvalArgs);
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
            static_assert(IsJsInteropable<ReturnType>::value);
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
    using ListArgsTail = tc::type::list<ArgsTail...>;
    using TailCCallableWrapper = CCallableWrapper<ListArgsTail, bPassedAllArguments>;
public:
    static_assert(!tc::type::find_unique<ListArgsTail, pass_this_t>::found);
    static_assert(IsJsInteropable<TThis>::value);

    static inline constexpr bool c_bInstantiated = true;

    template<typename Fn>
    emscripten::val operator()(Fn&& fn, emscripten::val const& emvalThis, emscripten::val const& emvalArgs) const& noexcept {
        return TailCCallableWrapper()(
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
    using ListArgsTail = tc::type::list<ArgsTail...>;
    using TailCCallableWrapper = CCallableWrapper<ListArgsTail, /*bPassedAllArguments=*/true>;
public:
    static_assert(!tc::type::find_unique<ListArgsTail, pass_this_t>::found);
    static_assert(!tc::type::find_unique<ListArgsTail, pass_all_arguments_t>::found);
    static_assert(IsJsInteropable<TArgs>::value);
    static_assert(!bPassedAllArguments);

    static inline constexpr bool c_bInstantiated = true;

    template<typename Fn>
    emscripten::val operator()(Fn&& fn, emscripten::val const& emvalThis, emscripten::val const& emvalArgs) const& noexcept {
        return TailCCallableWrapper()(
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
} // namespace callback_detail

namespace no_adl {
template<typename>
struct IJsFunction {};

template<typename R, typename... Args>
struct IJsFunction<R(Args...)> : virtual IJsBase {
    static_assert(IsJsInteropable<R>::value);
    // Implicit instantiation of CCallableWrapper to ensure Args are correct.
    static_assert(callback_detail::CCallableWrapper<tc::type::list<Args...>>::c_bInstantiated);

    R operator()(Args... args) noexcept {
        // These are limitations of emscripten::val, can be worked around.
        static_assert(std::is_same<tc::type::find_unique_if_result::type_not_found, tc::type::find_unique<tc::type::list<Args...>, pass_this_t>>::value, "Cannot call a JS function which needs 'this'");
        static_assert(std::is_same<tc::type::find_unique_if_result::type_not_found, tc::type::find_unique<tc::type::list<Args...>, pass_all_arguments_t>>::value, "Cannot call a JS function which takes an array of arguments");
        return _call_this<R>(tc_move(args)...);
    }
};
} // namespace no_adl
using no_adl::IJsFunction;

namespace callback_detail {
using FirstArgument = void*;
using FunctionPointer = emscripten::val(*)(FirstArgument, emscripten::val const& emvalThis, emscripten::val const& emvalArgs) noexcept;
using PointerNumber = std::uintptr_t;

namespace no_adl {
// See comments about memory correctness in js_callback.cpp.
struct RequireRelaxedPointerSafety {
    RequireRelaxedPointerSafety() noexcept {
        _ASSERT(std::get_pointer_safety() == std::pointer_safety::preferred || std::get_pointer_safety() == std::pointer_safety::relaxed);
    }
};

// We do not care about slicing to js_ref<>, because this class is
// only stored as a by-value field.
template<typename T>
struct CUniqueDetachableJsFunction : tc::nonmovable, RequireRelaxedPointerSafety, js_ref<IJsFunction<T>> { // TODO: private inheritance?
    CUniqueDetachableJsFunction(FunctionPointer pfunc, FirstArgument arg0) noexcept : js_ref<IJsFunction<T>>(
        emscripten::val::module_property("tc_js_callback_detail_js_CreateJsFunction")(reinterpret_cast<PointerNumber>(pfunc), reinterpret_cast<PointerNumber>(arg0))
    ) {}

    ~CUniqueDetachableJsFunction() {
        this->get().template call<void>("detach");
    }
};
} // namespace no_adl
using no_adl::CUniqueDetachableJsFunction;
} // namespace callback_detail

template<typename T>
struct IsJsRef<callback_detail::CUniqueDetachableJsFunction<T>> : std::true_type {
    using element_type = IJsFunction<T>;
};

#define TC_JS_MEMBER_FUNCTION(ClassName, FieldName, ReturnType, Arguments) \
    void FieldName##_tc_js_type_check() { \
        STATICASSERTSAME(ClassName&, decltype(*this), "ClassName specified in TC_JS_MEMBER_FUNCTION is incorrect"); \
    } \
    static emscripten::val FieldName##_tc_js_wrapper(void* pvThis, emscripten::val const& emvalThis, emscripten::val const& emvalArgs) noexcept { \
        return ::tc::js::callback_detail::MemberFunctionWrapper(&ClassName::FieldName##_tc_js_impl, pvThis, emvalThis, emvalArgs); \
    } \
    ::tc::js::callback_detail::CUniqueDetachableJsFunction<ReturnType Arguments> const FieldName{&FieldName##_tc_js_wrapper, this}; \
    ReturnType FieldName##_tc_js_impl Arguments noexcept

// ---------------------------------------- Lambda wrapper callback ----------------------------------------
// This callback wraps lambda (or any callable) so it can be immediately passed as js_ref<IJsFunction<...>>.
// It takes ownership of the lambda. Whenever the wrapper is destroyed, corresponding JS function is turned
// into no-op.
//
// Heap-allocated "fire-and-forget" callbacks are explicitly out of scope for the library: there are typically
// no guarantees on when they're called and we want the user to think about that carefully and make sure all
// proper cancellations are in place (e.g. by using member callback tied to the object which initiates the request).
namespace callback_detail {
template<typename Fn>
using js_lambda_wrap_base_t = callback_detail::CUniqueDetachableJsFunction<boost::callable_traits::function_type_t<Fn>>;
} // namespace callback_detail

namespace no_adl {
// We do not care about slicing to emscripten::val, because this class is only stored by-value.
template<typename Fn>
struct js_lambda_wrap final : callback_detail::js_lambda_wrap_base_t<Fn> {
    static_assert(!std::is_reference<Fn>::value);
    static_assert(boost::callable_traits::is_noexcept<Fn>::value, "Callbacks for JS should be noexcept");

    template<typename FnSrc>
    js_lambda_wrap(FnSrc&& fn) noexcept : callback_detail::js_lambda_wrap_base_t<Fn>(&FnWrapper, this), m_fn(std::forward<FnSrc>(fn)) {}

private:
    Fn m_fn;

    static emscripten::val FnWrapper(void* pThis, emscripten::val const& emvalThis, emscripten::val const& emvalArgs) noexcept {
        return callback_detail::CCallableWrapper<boost::callable_traits::args_t<Fn, tc::type::list>>()(
            tc::void_cast<js_lambda_wrap>(pThis)->m_fn,
            emvalThis,
            emvalArgs
        );
    }
};
template<typename Fn> js_lambda_wrap(Fn) -> js_lambda_wrap<Fn>;
} // namespace no_adl
using no_adl::js_lambda_wrap;

template<typename Fn>
struct IsJsRef<js_lambda_wrap<Fn>> : IsJsRef<callback_detail::js_lambda_wrap_base_t<Fn>> {};
} // namespace tc::js
