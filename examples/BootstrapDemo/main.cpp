#include <emscripten/val.h>
#include <vector>
#include <initializer_list>
#include <string>
#include <type_traits>
#include "explicit_cast.h"
#include "range.h"
#include "assert_defs.h"
#include "js_bootstrap.h"

using tc::js::string;
using tc::jst::js_unknown;
using tc::js::Array;
using tc::js::ReadonlyArray;
using tc::js::console;

enum class MyIntEnum { Foo = 10 };

enum class MyHeterogeneousEnum { Foo, Bar };

namespace tc::jst {
template<> struct IsJsIntegralEnum<MyIntEnum> : std::true_type {};
template<> struct IsJsHeterogeneousEnum<MyHeterogeneousEnum> : std::true_type {
	static inline auto const& Values() {
		static tc::unordered_map<MyHeterogeneousEnum, js_unknown> vals{
			{MyHeterogeneousEnum::Foo, 23.0},
			{MyHeterogeneousEnum::Bar, string("bar")}
		};
		return vals;
	}
};
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
		Array<string> const arr(tc::jst::create_js_object, std::initializer_list<char const*>{"Hello", "Hi!"});
		static_assert(std::is_same_v<tc::range_value_t<decltype(arr)>, string>);
		_ASSERTEQUAL(arr->length(), 2);
		_ASSERTEQUAL(arr[0].length(), 5);
		_ASSERTEQUAL(arr[1].length(), 3);
		_ASSERT(!tc::empty(arr));
	}

	{
	    constexpr char str[] = "Test";
        Array<string> const arr(tc::jst::create_js_object, tc::single(str));
        _ASSERTEQUAL(arr->length(), 1);
        _ASSERTEQUAL(arr[0].length(), 4);
	}
	{
	    constexpr char str[] = "Test";
        ReadonlyArray<string> const arr(tc::jst::create_js_object, tc::single(str));
        _ASSERTEQUAL(arr->length(), 1);
        _ASSERTEQUAL(arr[0].length(), 4);
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
	{
		// Check that BindingType<> removes cv-refs.
		auto const e = MyIntEnum::Foo;
		emscripten::val const emval(e);
		js_unknown const unk(e);
	}

	{
		emscripten::val const emval(MyHeterogeneousEnum::Foo);
		_ASSERT(emval.isNumber());
		_ASSERTEQUAL(emval.template as<double>(), 23.0);
		_ASSERTEQUAL(emval.template as<MyHeterogeneousEnum>(), MyHeterogeneousEnum::Foo);

		js_unknown const unk(MyHeterogeneousEnum::Foo);
		_ASSERTEQUAL(static_cast<MyHeterogeneousEnum>(unk), MyHeterogeneousEnum::Foo);
		_ASSERT(unk.getEmval().isNumber());
		_ASSERTEQUAL(unk.getEmval().template as<double>(), 23.0);
	}

	{
		emscripten::val const emval(MyHeterogeneousEnum::Bar);
		_ASSERT(emval.isString());
		_ASSERTEQUAL(emval.template as<std::string>(), "bar");
		_ASSERTEQUAL(emval.template as<MyHeterogeneousEnum>(), MyHeterogeneousEnum::Bar);

		js_unknown const unk(MyHeterogeneousEnum::Bar);
		_ASSERTEQUAL(static_cast<MyHeterogeneousEnum>(unk), MyHeterogeneousEnum::Bar);
		_ASSERT(unk.getEmval().isString());
		_ASSERTEQUAL(unk.getEmval().template as<std::string>(), "bar");
	}
	{
		// Check that BindingType<> removes cv-refs.
		auto e = MyHeterogeneousEnum::Foo;
		emscripten::val const emval(e);
		js_unknown const unk(e);
	}
	return 0;
}
