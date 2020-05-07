#pragma once
#include <emscripten/val.h>
#include <emscripten/wire.h>
#include <cstdint>
#include <memory>
#include <utility>
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

namespace tc::jst {
// ---------------------------------------- Passing member functions to JS ----------------------------------------
DEFINE_TAG_TYPE(pass_this)
DEFINE_TAG_TYPE(pass_all_arguments)

namespace callback_detail {
void EnsureJsCallbackCppIsLinked();

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
			_ASSERTEQUAL(emvalArgs["length"].as<std::size_t>(), tc::type::size<ListArgs>::value);
		}
		return [&]<typename... Args, std::size_t... Indices>(tc::type::list<Args...>, std::index_sequence<Indices...>) {
			auto fnWithArgs = [&]() noexcept -> decltype(auto) {
				return std::forward<Fn>(fn)(emvalArgs[Indices].template as<Args>()...);
			};
			using ReturnType = decltype(fnWithArgs());
			static_assert(IsJsInteropable<ReturnType>::value);
			if constexpr (std::is_same<void, ReturnType>::value) {
				fnWithArgs();
				return emscripten::val::undefined();
			} else {
				return emscripten::val(fnWithArgs());
			}
		}(ListArgs{}, std::make_index_sequence<tc::type::size<ListArgs>::value>{});
	}
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
			[&](auto&&... args) noexcept -> decltype(auto) {
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
			[&](auto&&... args) noexcept -> decltype(auto) {
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
		[&](auto&&... args) noexcept -> decltype(auto) {
			return (tc::void_cast<ThisType>(pvThis)->*pmfMember)(std::forward<decltype(args)>(args)...);
		}, emvalThis, emvalArgs
	);
}
} // namespace callback_detail

namespace no_adl {
template<typename>
struct IJsFunction {};

template<typename R, typename... Args>
struct IJsFunction<R(Args...)> : virtual IObject {
	struct _tcjs_definitions {
		using function_type = R(Args...);
	};

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
template<typename T> using js_function = js_ref<IJsFunction<T>>;

namespace callback_detail {
using FirstArgument = void*;
using FunctionPointer = emscripten::val(*)(FirstArgument, emscripten::val const& emvalThis, emscripten::val const& emvalArgs) noexcept;
using PointerNumber = std::uintptr_t;

namespace no_adl {
// See comments about memory correctness in js_callback.cpp.
struct RequireRelaxedPointerSafety {
	RequireRelaxedPointerSafety() noexcept {
		_ASSERT(std::pointer_safety::preferred == std::get_pointer_safety() || std::pointer_safety::relaxed == std::get_pointer_safety());
	}
};

// We do not care about slicing to js_ref<> or moving from this class, because
// it is only stored as a by-value const field.
template<typename T>
struct CUniqueDetachableJsFunction : private tc::nonmovable, private RequireRelaxedPointerSafety, js_function<T> {
	CUniqueDetachableJsFunction(FunctionPointer pfunc, FirstArgument arg0) noexcept : js_function<T>([&]() {
		static auto creator = []() {
			EnsureJsCallbackCppIsLinked();
			auto creator = emscripten::val::module_property("tc_js_callback_detail_js_CreateJsFunction");
			_ASSERT(!creator.isUndefined() && "Unable to find a function from js_callback.js, did you pass '--pre-js js_callback.js' flags to em++?");
			return creator;
		}();
		return creator(reinterpret_cast<PointerNumber>(pfunc), reinterpret_cast<PointerNumber>(arg0));
	}()) {}

	~CUniqueDetachableJsFunction() noexcept {
		this->getEmval().template call<void>("detach");
	}
};
} // namespace no_adl
using no_adl::CUniqueDetachableJsFunction;
} // namespace callback_detail

#define TC_JS_MEMBER_FUNCTION(ClassName, FieldName, ReturnType, Arguments) \
	void FieldName##_tc_js_type_check() noexcept { \
		STATICASSERTSAME(ClassName&, decltype(*this), "ClassName specified in TC_JS_MEMBER_FUNCTION is incorrect"); \
	} \
	static emscripten::val FieldName##_tc_js_wrapper(void* pvThis, emscripten::val const& emvalThis, emscripten::val const& emvalArgs) noexcept { \
		return ::tc::jst::callback_detail::MemberFunctionWrapper(&ClassName::FieldName##_tc_js_impl, pvThis, emvalThis, emvalArgs); \
	} \
	::tc::jst::callback_detail::CUniqueDetachableJsFunction<ReturnType Arguments> const FieldName{&FieldName##_tc_js_wrapper, this}; \
	ReturnType FieldName##_tc_js_impl Arguments noexcept

// ---------------------------------------- Lambda wrapper callback ----------------------------------------
// This callback wraps lambda (or any callable) so it can be immediately passed as js_function<...>.
// It takes ownership of the lambda. Whenever the wrapper is destroyed, corresponding JS function is turned
// into no-op.
//
// Heap-allocated "fire-and-forget" callbacks are explicitly out of scope for the library: there are typically
// no guarantees on when they're called and we want the user to think about that carefully and make sure all
// proper cancellations are in place (e.g. by using member callback tied to the object which initiates the request).
namespace no_adl {
template<typename Fn>
struct js_lambda_wrap final : private tc::nonmovable {
	using function_type = boost::callable_traits::function_type_t<Fn>;

	static_assert(!std::is_reference<Fn>::value);
	static_assert(boost::callable_traits::is_noexcept<Fn>::value, "Callbacks for JS should be noexcept");

	template<typename FnSrc>
	js_lambda_wrap(FnSrc&& fn) noexcept : m_fn(std::forward<FnSrc>(fn)), m_jsFunction(&FnWrapper, this) {}

	// Explicitly disable two-way conversion enabled by IsEmvalWrapper below.
	js_lambda_wrap(emscripten::val) = delete;

	// Always return a copy of `m_emval` and keep ownership even if we're rvalue.
	operator js_function<function_type>() const { return m_jsFunction; }

	emscripten::val getEmval() const {
		return m_jsFunction.getEmval();
	}

private:
	Fn m_fn;
	callback_detail::CUniqueDetachableJsFunction<function_type> const m_jsFunction;

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

namespace no_adl {
template<typename T>
struct IsJsInteropable<T, std::enable_if_t<
	tc::is_instance_or_derived<js_lambda_wrap, T>::value
>> : std::true_type {};
} // namespace no_adl

namespace emscripten_interop_detail::no_adl {
template<typename T>
struct IsEmvalWrapper<T, std::enable_if_t<tc::is_instance_or_derived<js_lambda_wrap, T>::value>> : std::true_type {
};
} // namespace emscripten_interop_detail::no_adl
} // namespace tc::jst
