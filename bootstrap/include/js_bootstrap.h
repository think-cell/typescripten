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
struct _js_Console;

template<typename T> using Array = js_types::js_ref<_js_Array<T>>;
template<typename T> using ReadonlyArray = js_types::js_ref<_js_ReadonlyArray<T>>;
using Console = js_types::js_ref<_js_Console>;

template<typename T>
struct _js_Array : virtual js_types::IObject {
	static_assert(js_types::IsJsInteropable<T>::value);

	struct _tcjs_definitions {
		using value_type = T;
	};

	auto length() noexcept { return tc::explicit_cast<int>(_getProperty<double>("length")); }

	auto push(T const& item) noexcept { return _call<void>("push", item); }

	auto operator[](int i) && noexcept { return _getProperty<T>(i); }

	void _setIndex(int i, T value) noexcept { _setProperty(i, tc_move(value)); }

	// Generator range. This adds operator() to array interface (which did not exist before), but it's ok.
	template<typename Fn>
	auto operator()(Fn fn) noexcept {
		if (_call<bool>("some", js_types::js_lambda_wrap([&](T value, js_types::js_unknown, js_types::js_unknown) noexcept {
			return tc::break_ == tc::continue_if_not_break(fn, tc_move(value));
		})))
			return tc::break_;
		else
			return tc::continue_;
	}

	static Array<T> _tcjs_construct() noexcept {
		return Array<T>(emscripten::val::array());
	}

	template<typename Rng, typename = std::enable_if_t<tc::is_explicit_castable<T, tc::range_value_t<Rng>&&>::value>>
	static Array<T> _tcjs_construct(Rng&& rng) noexcept {
		Array<T> result(emscripten::val::array());
		tc::for_each(rng, [&](auto&& value) noexcept {
			result->push(tc::explicit_cast<T>(std::forward<decltype(value)>(value)));
		});
		return result;
	}
};

template<typename T>
struct _js_ReadonlyArray : virtual js_types::IObject {
	static_assert(js_types::IsJsInteropable<T>::value);

	struct _tcjs_definitions {
		using value_type = T;
	};

	auto length() noexcept { return tc::explicit_cast<int>(_getProperty<double>("length")); }

	auto operator[](int i) noexcept { return _getProperty<T>(i); }

	// Generator range. This adds operator() to array interface (which did not exist before), but it's ok.
	template<typename Fn>
	auto operator()(Fn fn) noexcept {
		if (_call<bool>("some", js_types::js_lambda_wrap([&](T value, js_types::js_unknown, js_types::js_unknown) noexcept {
			return tc::break_ == tc::continue_if_not_break(fn, tc_move(value));
		})))
			return tc::break_;
		else
			return tc::continue_;
	}

	static ReadonlyArray<T> _tcjs_construct() noexcept {
		return ReadonlyArray<T>(emscripten::val::array());
	}

	template<typename Rng, typename = std::enable_if_t<tc::is_explicit_castable<T, tc::range_value_t<Rng>&&>::value>>
	static ReadonlyArray<T> _tcjs_construct(Rng&& rng) noexcept {
		return ReadonlyArray<T>(
			Array<T>(js_types::create_js_object, std::forward<Rng>(rng)).getEmval()
		);
	}
};

struct _js_Console : virtual js_types::IObject {
	template<typename... Args>
	auto log(Args&&... args) noexcept {
		static_assert((js_types::IsJsInteropable<tc::remove_cvref_t<Args>>::value && ...));
		return _call<void>("log", std::forward<Args>(args)...);
	}
};
} // namespace no_adl

using no_adl::Array;
using no_adl::ReadonlyArray;
using no_adl::Console;

inline auto console() noexcept { return Console(emscripten::val::global("console")); }

} // namespace tc::js
