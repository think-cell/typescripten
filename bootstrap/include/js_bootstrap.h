#pragma once

#include <emscripten/val.h>
#include <utility>
#include <type_traits>
#include "break_or_continue.h"
#include "explicit_cast.h"
#include "type_traits.h"
#include "range.h"
#include "js_types.h"
#include "js_callback.h"
#include "tc_move.h"

namespace tc::js {

namespace no_adl {
template<typename> struct _js_Array;
template<typename> struct _js_ReadonlyArray;
template<typename> struct _js_Promise;
struct _js_console;

template<typename T> using Array = ::tc::jst::js_ref<_js_Array<T>>;
template<typename T> using ReadonlyArray = ::tc::jst::js_ref<_js_ReadonlyArray<T>>;
template<typename T> using Promise = ::tc::jst::js_ref<_js_Promise<T>>;
using console = ::tc::jst::js_ref<_js_console>;

template<typename T>
struct _js_Array : virtual ::tc::jst::IObject {
	static_assert(::tc::jst::IsJsInteropable<T>::value);

	struct _tcjs_definitions {
		using value_type = T;
	};

	auto length() noexcept { return ::tc::explicit_cast<int>(_getProperty<double>("length")); }

	auto push(T const& item) noexcept { return _call<void>("push", item); }

	auto operator[](int i) && noexcept { return _getProperty<T>(::tc::as_dec(i)); }

	void _setIndex(int i, T value) noexcept { _setProperty(::tc::as_dec(i), tc_move(value)); }

	// Generator range. This adds operator() to array interface (which did not exist before), but it's ok.
	template<typename Fn>
	auto operator()(Fn fn) noexcept {
		if (_call<bool>("some", ::tc::jst::js_lambda_wrap([&](T value, ::tc::jst::js_unknown, ::tc::jst::js_unknown) noexcept {
			return ::tc::break_ == tc::continue_if_not_break(fn, tc_move(value));
		})))
			return ::tc::break_;
		else
			return ::tc::continue_;
	}

	static Array<T> _tcjs_construct() noexcept {
		return Array<T>(::emscripten::val::array());
	}

	template<typename Rng, typename = ::std::enable_if_t<::tc::is_explicit_castable<T, ::tc::range_reference_t<Rng>>::value>>
	static Array<T> _tcjs_construct(Rng&& rng) noexcept {
		Array<T> result(::emscripten::val::array());
		::tc::for_each(::std::forward<Rng>(rng), [&](auto&& value) noexcept {
			result->push(::tc::explicit_cast<T>(::std::forward<decltype(value)>(value)));
		});
		return result;
	}

};

template<typename T>
struct _js_ReadonlyArray : virtual ::tc::jst::IObject {
	static_assert(::tc::jst::IsJsInteropable<T>::value);

	struct _tcjs_definitions {
		using value_type = T;
	};

	auto length() noexcept { return ::tc::explicit_cast<int>(_getProperty<double>("length")); }

	auto operator[](int i) noexcept { return _getProperty<T>(::tc::as_dec(i)); }

	// Generator range. This adds operator() to array interface (which did not exist before), but it's ok.
	template<typename Fn>
	auto operator()(Fn fn) noexcept {
		if (_call<bool>("some", ::tc::jst::js_lambda_wrap([&](T value, ::tc::jst::js_unknown, ::tc::jst::js_unknown) noexcept {
			return ::tc::break_ == ::tc::continue_if_not_break(fn, tc_move(value));
		})))
			return ::tc::break_;
		else
			return ::tc::continue_;
	}

	static ReadonlyArray<T> _tcjs_construct() noexcept {
		return ReadonlyArray<T>(::emscripten::val::array());
	}

	template<typename Rng, typename = ::std::enable_if_t<::tc::is_explicit_castable<T, ::tc::range_value_t<Rng>&&>::value>>
	static ReadonlyArray<T> _tcjs_construct(Rng&& rng) noexcept {
		return ReadonlyArray<T>(
			Array<T>(::tc::jst::create_js_object, ::std::forward<Rng>(rng)).getEmval()
		);
	}
};

template<typename T> struct RemovePromise { using type = T; };
template<typename T> struct RemovePromise<Promise<T>> { using type = T; };
template<typename T> using RemovePromise_t = typename RemovePromise<T>::type;

template<typename T>
struct _js_Promise : virtual ::tc::jst::IObject {
	static_assert(::tc::jst::IsJsInteropable<T>::value);

	template<typename R>
	auto then(::tc::jst::js_function<R(T)> onfulfilled, ::tc::jst::js_function<R(::tc::jst::js_unknown)> onrejected) noexcept {
		return _call<Promise<RemovePromise_t<R>>>("then", onfulfilled, onrejected);
	}

	template<typename R1>
	auto then(::tc::jst::js_function<R1(T)> onfulfilled) noexcept {
		return _call<Promise<RemovePromise_t<R1>>>("then", onfulfilled);
	}

	template<typename R1, typename R2>
	auto then(::tc::jst::js_function<R1(T)> onfulfilled, ::tc::jst::js_function<R2(jst::js_unknown)> onrejected) noexcept {
		return _call<Promise<::tc::jst::js_union<RemovePromise_t<R1>, RemovePromise_t<R2>>>>("then", onfulfilled, onrejected);
	}
};

template<>
struct _js_Promise<void> : virtual _js_Promise<::tc::jst::js_undefined> {
	// JavaScript passes 'undefined' to what TypeScript calls 'void' promise.
};

struct _js_console : virtual ::tc::jst::IObject {
	struct _tcjs_definitions {
		template<typename... Args>
		static void log(Args&&... args) noexcept {
			static_assert((::tc::jst::IsJsInteropable<::tc::remove_cvref_t<Args>>::value && ...));
			::emscripten::val::global("console")["log"](::std::forward<Args>(args)...);
		}

		template<typename... Args>
		static void error(Args&&... args) noexcept {
			static_assert((::tc::jst::IsJsInteropable<::tc::remove_cvref_t<Args>>::value && ...));
			::emscripten::val::global("console")["error"](::std::forward<Args>(args)...);
		}

		template<typename... Args>
		static void warn(Args&&... args) noexcept {
			static_assert((::tc::jst::IsJsInteropable<::tc::remove_cvref_t<Args>>::value && ...));
			::emscripten::val::global("console")["warn"](::std::forward<Args>(args)...);
		}

		template<typename... Args>
		static void debug(Args&&... args) noexcept {
			static_assert((::tc::jst::IsJsInteropable<::tc::remove_cvref_t<Args>>::value && ...));
			::emscripten::val::global("console")["debug"](::std::forward<Args>(args)...);
		}
	};
};
} // namespace no_adl

using no_adl::Array;
using no_adl::ReadonlyArray;
using no_adl::Promise;
using no_adl::console;

} // namespace tc::js
