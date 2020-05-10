#include <emscripten/val.h>
#include <vector>
#include <initializer_list>
#include <type_traits>
#include "explicit_cast.h"
#include "range.h"
#include "range_defines.h"
#include "js_bootstrap.h"

using tc::jst::js_string;
using tc::jst::js_unknown;
using tc::js::Array;
using tc::js::ReadonlyArray;
using tc::js::console;

enum class MyIntEnum { Foo = 10 };

namespace tc::jst {
template<> struct IsJsIntegralEnum<MyIntEnum> : std::true_type {};
} // namespace tc::jst

int main() {
	{
		auto const arr = tc::explicit_cast<ReadonlyArray<double>>(tc::jst::create_js_object, std::initializer_list<double>{1, 2, 3});
		static_assert(!tc::is_explicit_castable<ReadonlyArray<double>, double>::value);
		static_assert(std::is_same_v<tc::range_value_t<ReadonlyArray<double>>, double>);
		console::log(arr);
		_ASSERTEQUAL(arr->length(), 3);
		_ASSERTEQUAL(arr[0], 1);
		_ASSERTEQUAL(arr[1], 2);
		_ASSERTEQUAL(arr[2], 3);
	}

	{
		Array<double> const arr(tc::jst::create_js_object);
		_ASSERT(tc::empty(arr));
		_ASSERTEQUAL(arr->length(), 0);
		arr->push(10);
		_ASSERT(!tc::empty(arr));
		_ASSERTEQUAL(arr->length(), 1);
		_ASSERTEQUAL(arr[0], 10);
	}

	{
		Array<js_string> const arr(tc::jst::create_js_object, std::initializer_list<char const*>{"Hello", "Hi!"});
		static_assert(std::is_same_v<tc::range_value_t<decltype(arr)>, js_string>);
		_ASSERTEQUAL(arr->length(), 2);
		_ASSERTEQUAL(arr[0].length(), 5);
		_ASSERTEQUAL(arr[1].length(), 3);
		_ASSERT(!tc::empty(arr));
	}

	auto const arr = tc::explicit_cast<Array<double>>(tc::jst::create_js_object, std::initializer_list<double>{1, 2, 3});
	static_assert(!tc::is_explicit_castable<Array<double>, double>::value);
	static_assert(std::is_same_v<tc::range_value_t<Array<double>>, double>);
	console::log(arr);
	_ASSERTEQUAL(arr->length(), 3);
	_ASSERTEQUAL(arr[1], 2);
	arr->_setIndex(1, 15);
	_ASSERTEQUAL(arr[1], 15);

	{
		std::vector<double> result;
		tc::for_each(arr, [&](double item) noexcept { result.push_back(item); });
		_ASSERTEQUAL(result, (std::vector{1.0, 15.0, 3.0}));
	}

	{
		std::vector<double> result;
		tc::for_each(
			tc::filter(
				tc::transform(arr,
					[](double x) noexcept { return x * x; }
				),
				[](double x) noexcept { return x >= 2; }
			),
			[&](double item) noexcept { result.push_back(item); }
		);
		_ASSERTEQUAL(result, (std::vector{225.0, 9.0}));
	}

	{
		emscripten::val const emval(MyIntEnum::Foo);
		_ASSERT(emval.isNumber());
		_ASSERTEQUAL(emval.template as<double>(), 10.0);
		_ASSERTEQUAL(emval.template as<MyIntEnum>(), MyIntEnum::Foo);

		js_unknown const unk(MyIntEnum::Foo);
		_ASSERTEQUAL(static_cast<MyIntEnum>(unk), MyIntEnum::Foo);
		_ASSERT(unk.getEmval().isNumber());
		_ASSERTEQUAL(unk.getEmval().template as<double>(), 10.0);
	}
	return 0;
}
