#pragma once

#include <typescripten/typescripten.h>


JS_RANGE_WITH_ITERATORS_TMPL(tc::js, ReadonlyArray)
JS_RANGE_WITH_ITERATORS_TMPL(tc::js, Array)

namespace tc::jst {
	template<typename T, typename Rng>
	tc::js::Array<T> make_Array(Rng&& rng) noexcept {
		tc::js::Array<T> result(::emscripten::val::array());
		::tc::for_each(::std::forward<Rng>(rng), [&](auto&& value) noexcept {
			result->push(::tc::jst::variadic_arg, ::tc::explicit_cast<T>(::std::forward<decltype(value)>(value)));
		});
		return result;
	}

	template<typename T, typename Rng>
	tc::js::ReadonlyArray<T> make_ReadonlyArray(Rng&& rng) noexcept {
		return tc::js::ReadonlyArray<T>(make_Array<T>(std::forward<Rng>(rng)).getEmval());
	}
}