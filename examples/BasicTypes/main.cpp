#include <emscripten/val.h>
#include <type_traits>
#include "explicit_cast.h"
#include "range.h"
#include "assert_defs.h"
#include "js_types.h"
#include "js_ref.h"

namespace js = tc::js;
namespace jst = tc::jst;

struct ISomeObject : virtual jst::IObject {
	struct _tcjs_definitions { // Optional
		using Foo = int;
	};

	auto foo() {
		return _call<js::string>("foo");
	}

	auto operator()(double) {
	    return _call_this<void>();
	}

	auto operator()(js::string) {
	    return _call_this<double>();
	}
};

using SomeObject = jst::js_ref<ISomeObject>;

int main() {
	{
		static_assert(std::is_same<int, SomeObject::Foo>::value);
		static_assert(std::is_same<js::string, decltype(std::declval<SomeObject>()->foo())>::value);
		static_assert(std::is_same<void, decltype(std::declval<SomeObject>()(10.0))>::value);
		static_assert(std::is_same<double, decltype(std::declval<SomeObject>()(js::string("hi")))>::value);
	}

	// any
	{
		static_assert(std::is_convertible<double, js::any>::value);
		static_assert(std::is_convertible<bool, js::any>::value);
		static_assert(std::is_convertible<js::string, js::any>::value);
		static_assert(std::is_convertible<js::null, js::any>::value);
		static_assert(std::is_convertible<js::undefined, js::any>::value);

		static_assert(!std::is_convertible<std::string, js::any>::value);
		static_assert(!std::is_convertible<char, js::any>::value);
		static_assert(!std::is_convertible<float, js::any>::value);
		static_assert(!std::is_convertible<int, js::any>::value);

		// Explicit casting to supported types only, no implicit conversion
		static_assert(tc::is_explicit_castable<bool, js::any>::value); // any -> bool conversion checks at runtime that any stores a bool, no JS truthiness
		static_assert(tc::is_explicit_castable<double, js::any>::value);
		static_assert(tc::is_explicit_castable<js::null, js::any>::value);
		static_assert(tc::is_explicit_castable<js::undefined, js::any>::value);
		static_assert(tc::is_explicit_castable<js::string, js::any>::value);

		static_assert(!std::is_convertible<js::any, bool>::value);
		static_assert(!std::is_convertible<js::any, double>::value);
		static_assert(!std::is_convertible<js::any, js::null>::value);
		static_assert(!std::is_convertible<js::any, js::undefined>::value);
		static_assert(!std::is_convertible<js::any, js::string>::value);

		static_assert(!tc::is_explicit_castable<int, js::any>::value);
		static_assert(!std::is_convertible<js::any, int>::value);
	}
	{
		// Emscripten supports reading an int from a double, so we support doubles only, like JS
		emscripten::val const x(1.23);
		_ASSERTEQUAL(x.as<int>(), 1);

		// We also do not expose the JS semantics to conversion-to-bool
		_ASSERT(x.as<bool>());

		emscripten::val const y(0);
		_ASSERT(!y.as<bool>());

		emscripten::val const z(std::string("Hello"));
		_ASSERT(z.as<bool>());

		emscripten::val const v(std::string(""));
		_ASSERT(!v.as<bool>());
	}
	{
		js::any a = true;
		js::any b = false;
		_ASSERT(tc::explicit_cast<bool>(a) && !tc::explicit_cast<bool>(b));
	}

	// string
	{
		// Constructible from anything we can create a std::string from
		static_assert(std::is_constructible<js::string, char const*>::value);
		static_assert(std::is_constructible<js::string, std::string>::value);
		static_assert(!std::is_constructible<js::string, double>::value);
		static_assert(!std::is_constructible<js::string, int>::value);

		// Implicit conversion string -> any
		static_assert(std::is_convertible<js::string, js::any>::value);
		static_assert(!std::is_convertible<js::any, js::string>::value);

		// Explicit conversion any -> string
		static_assert(tc::is_explicit_castable<js::string, js::any>::value);

		// Neither implicit nor explicit conversion to bool. No JS truthiness of strings.
		static_assert(!std::is_convertible<js::string, bool>::value);
		static_assert(!tc::is_explicit_castable<bool, js::string>::value);
	
		js::string message("Hello World");
		_ASSERTEQUAL(message.length(), 11);
		_ASSERTEQUAL(tc::explicit_cast<std::string>(message), "Hello World");

		// Implicit base-cast.
		js::any anyMessage = message;		
		_ASSERTEQUAL(message.length(), 11);
		_ASSERT(message.getEmval().strictlyEquals(anyMessage.getEmval()));
		
		anyMessage = message;
		_ASSERTEQUAL(message.length(), 11);
		_ASSERT(message.getEmval().strictlyEquals(anyMessage.getEmval()));

		// Explicit derived-cast.
		js::string const message2(anyMessage);
		_ASSERT(message.getEmval().strictlyEquals(message2.getEmval()));
	}

	{
	    auto const message = jst::make_string("Hello", tc::as_dec(10), "world");
	    static_assert(std::is_same<const js::string, decltype(message)>::value);
	    _ASSERTEQUAL(message.length(), 12);
	    _ASSERTEQUAL(tc::explicit_cast<std::string>(message), "Hello10world");
	}

	{
		emscripten::val const emval{js::undefined{}};
		_ASSERTE(emval.isUndefined());
		js::undefined{emval};
	}

	{
		emscripten::val const emval{js::null{}};
		_ASSERTE(emval.isNull());
		js::null{emval};
	}

	{
		// Test optional of object.
		using OptionalUnknown = jst::optional<jst::js_object>;
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
			jst::js_object const jsOrigin(emscripten::val::object());
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
			jst::optional<double> const iValue;
			emscripten::val const emval(iValue);
			_ASSERT(emval.isUndefined());
			_ASSERT(!emval.isNumber());
		}
		{
			jst::optional<double> const iValue(123.5);
			emscripten::val const emval(iValue);
			_ASSERT(!emval.isUndefined());
			_ASSERT(emval.isNumber());
			_ASSERT(123.5 == emval.as<double>());
		}
	}

	
	return 0;
}
