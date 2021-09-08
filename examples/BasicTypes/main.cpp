#include <emscripten/val.h>
#include <type_traits>
#include "explicit_cast.h"
#include "range.h"
#include "assert_defs.h"
#include "js_types.h"
#include "js_ref.h"

using tc::js::string;
using tc::js::any;
using tc::jst::js_optional;

struct ISomeObject : virtual tc::jst::IObject {
	struct _tcjs_definitions { // Optional
		using Foo = int;
	};

	auto foo() {
		return _call<string>("foo");
	}

	auto operator()(double) {
	    return _call_this<void>();
	}

	auto operator()(string) {
	    return _call_this<double>();
	}
};

using SomeObject = tc::jst::js_ref<ISomeObject>;

int main() {
	{
		static_assert(std::is_same<int, SomeObject::Foo>::value);
		static_assert(std::is_same<string, decltype(std::declval<SomeObject>()->foo())>::value);
		static_assert(std::is_same<void, decltype(std::declval<SomeObject>()(10.0))>::value);
		static_assert(std::is_same<double, decltype(std::declval<SomeObject>()(string("hi")))>::value);
	}

	{
		auto const message = tc::explicit_cast<string>("Hello World");
		static_assert(!std::is_constructible<string, double>::value);
		_ASSERTEQUAL(message.length(), 11);
		_ASSERTEQUAL(tc::explicit_cast<std::string>(message), "Hello World");

		// Implicit basecast.
		any anyMessage = message;
		static_assert(std::is_convertible<string, any>::value);
		_ASSERTEQUAL(message.length(), 11);
		_ASSERT(message.getEmval().strictlyEquals(anyMessage.getEmval()));
		anyMessage = message;
		_ASSERTEQUAL(message.length(), 11);
		_ASSERT(message.getEmval().strictlyEquals(anyMessage.getEmval()));

		// Explicit derivedcast.
		string const message2(anyMessage);
		static_assert(!std::is_convertible<any, string>::value);
		_ASSERT(message.getEmval().strictlyEquals(message2.getEmval()));
	}

	{
	    auto const message = tc::jst::make_string("Hello", tc::as_dec(10), "world");
	    static_assert(std::is_same<const string, decltype(message)>::value);
	    _ASSERTEQUAL(message.length(), 12);
	    _ASSERTEQUAL(tc::explicit_cast<std::string>(message), "Hello10world");
	}

	{
		emscripten::val const emval{tc::js::undefined{}};
		_ASSERTE(emval.isUndefined());
		tc::js::undefined{emval};
	}

	{
		emscripten::val const emval{tc::js::null{}};
		_ASSERTE(emval.isNull());
		tc::js::null{emval};
	}

	{
		// Test optional of object.
		using OptionalUnknown = js_optional<tc::jst::js_object>;
		static_assert(
			std::is_same<
				typename emscripten::internal::BindingType<OptionalUnknown>::WireType,
				typename emscripten::internal::BindingType<emscripten::val>::WireType
			>::value);

		{
			emscripten::val const emval{OptionalUnknown()};
			_ASSERT(!emval.template as<OptionalUnknown>());
			_ASSERT(emval.isUndefined());
		}
		{
			tc::jst::js_object const jsOrigin(emscripten::val::object());
			emscripten::val emval{OptionalUnknown(jsOrigin)};
			auto const ounkParsed = emval.template as<OptionalUnknown>();
			_ASSERT(ounkParsed);
			_ASSERT(ounkParsed->getEmval().strictlyEquals(jsOrigin.getEmval()));
			_ASSERT(!emval.isUndefined());
			_ASSERT(emval.strictlyEquals(jsOrigin.getEmval()));
		}
		{
			OptionalUnknown const ounk;
			emscripten::val const emval(ounk);
			_ASSERT(!emval.template as<OptionalUnknown>());
			_ASSERT(emval.isUndefined());
		}
	}

	{
		// Test optional of double.
		{
			js_optional<double> const iValue;
			emscripten::val const emval(iValue);
			_ASSERT(emval.isUndefined());
			_ASSERT(!emval.isNumber());
		}
		{
			js_optional<double> const iValue(123.5);
			emscripten::val const emval(iValue);
			_ASSERT(!emval.isUndefined());
			_ASSERT(emval.isNumber());
			_ASSERT(123.5 == emval.as<double>());
		}
	}

	{
		// It works, that's why we prohibit int.
		emscripten::val const x(1.23);
		_ASSERTEQUAL(x.as<int>(), 1);
	}
	return 0;
}
