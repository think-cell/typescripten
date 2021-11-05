#include <emscripten/val.h>
#include <iostream>
#include <type_traits>
#include "explicit_cast.h"
#include "type_traits.h"
#include "range.h"
#include "assert_defs.h"
#include "js_callback.h"
#include "js_bootstrap.h"

using tc::jst::create_js_object;
using tc::js::string;
using tc::js::any;
using tc::jst::function;
using tc::jst::pass_this_t;
using tc::jst::pass_all_arguments_t;
using tc::js::Array;

struct _js_SomeJsClass : virtual tc::jst::object_base {
	auto intValue() noexcept { return tc::explicit_cast<int>(_getProperty<double>("intValue")); }

	static auto _tcjs_construct(int v) noexcept {
		return emscripten::val::module_property("SomeJsClass").new_(v);
	}
};
using SomeJsClass = tc::jst::ref<_js_SomeJsClass>;

int callback_counter;

#define FOR_ALL_CALLBACKS(CreateCallback) \
	CreateCallback(TestSum, double, (double a, double b), { \
		callback_counter++; \
		_ASSERTEQUAL(a, 10.0); \
		_ASSERTEQUAL(b, 20.0); \
		return a + b; \
	}) \
	CreateCallback(TestStringPass, string, (string const sMessage), { \
		callback_counter++; \
		_ASSERTEQUAL(tc::explicit_cast<std::string>(sMessage), "hello"); \
		return string(tc::explicit_cast<std::string>(sMessage) + " world"); \
	})\
	CreateCallback(TestPassAllArguments, void, (pass_all_arguments_t, Array<any> const jsarrunkArgs, double a), { \
		callback_counter++; \
		_ASSERTEQUAL(a, 1.0); \
		_ASSERTEQUAL(tc::explicit_cast<std::string>(string(jsarrunkArgs[1])), "message"); \
		_ASSERTEQUAL(jsarrunkArgs->length(), 3); \
	}) \
	CreateCallback(TestPassThis, void, (pass_this_t, SomeJsClass const jssjcThis, double a, string const b, any const c), { \
		callback_counter++; \
		_ASSERTEQUAL(jssjcThis->intValue(), 10); \
		_ASSERTEQUAL(a, 1.0); \
		_ASSERTEQUAL(tc::explicit_cast<std::string>(b), "message"); \
		_ASSERT(c.getEmval().isUndefined()); \
	}) \
	CreateCallback(TestPassThisPassAllArguments, void, (pass_this_t, SomeJsClass const jssjcThis, pass_all_arguments_t, Array<any> const jsarrunkArgs, double a), { \
		callback_counter++; \
		_ASSERTEQUAL(jssjcThis->intValue(), 10); \
		_ASSERTEQUAL(a, 1.0); \
		_ASSERTEQUAL(tc::explicit_cast<std::string>(string(jsarrunkArgs[1])), "message"); \
		_ASSERTEQUAL(jsarrunkArgs->length(), 3); \
	}) \
	CreateCallback(TestPassAllArgumentsAndReturn, SomeJsClass, (pass_all_arguments_t, Array<any> const jsarrunkArgs, double a), { \
		callback_counter++; \
		_ASSERTEQUAL(a, 1.0); \
		_ASSERTEQUAL(tc::explicit_cast<std::string>(string(jsarrunkArgs[1])), "message"); \
		_ASSERTEQUAL(jsarrunkArgs->length(), 3); \
		return SomeJsClass(create_js_object, 123); \
	}) \
	CreateCallback(TestPassThisAndReturn, SomeJsClass, (pass_this_t, SomeJsClass const jssjcThis, double a, string const b, any const c), { \
		callback_counter++; \
		_ASSERTEQUAL(jssjcThis->intValue(), 10); \
		_ASSERTEQUAL(a, 1.0); \
		_ASSERTEQUAL(tc::explicit_cast<std::string>(b), "message"); \
		_ASSERT(c.getEmval().isUndefined()); \
		return SomeJsClass(create_js_object, 123); \
	}) \
	CreateCallback(TestPassThisPassAllArgumentsAndReturn, SomeJsClass, (pass_this_t, SomeJsClass const jssjcThis, pass_all_arguments_t, Array<any> const jsarrunkArgs, double a), { \
		callback_counter++; \
		_ASSERTEQUAL(jssjcThis->intValue(), 10); \
		_ASSERTEQUAL(a, 1.0); \
		_ASSERTEQUAL(tc::explicit_cast<std::string>(string(jsarrunkArgs[1])), "message"); \
		_ASSERTEQUAL(jsarrunkArgs->length(), 3); \
		return SomeJsClass(create_js_object, 123); \
	})

template<typename T, typename U>
void ensureJsFunctionDeduction(function<U>) {
	static_assert(std::is_same<T, function<U>>::value);
}

// Macro so callback creation is performed between "start" and "end".
#define RUN_TEST(Name, ExpectedType, Callback) \
	{ \
		std::cout << #Name " start\n"; \
		auto&& cb = Callback; \
		using Fn = decltype(cb); \
		static_assert(std::is_same< \
			typename tc::remove_cvref_t<Fn>::function_type, \
			typename ExpectedType::function_type \
		>::value, #Name); \
		static_assert(std::is_convertible< \
			tc::remove_cvref_t<Fn>, \
			ExpectedType \
		>::value, #Name); \
		ensureJsFunctionDeduction<ExpectedType>(cb); \
		int callback_counter_before = callback_counter; \
		emscripten::val::module_property(#Name)(std::forward<Fn>(cb)); \
		_ASSERTEQUAL(callback_counter, callback_counter_before + 1); \
		std::cout << #Name " end\n"; \
	}

int main() {
	{
		std::cout << "===== lambda =====\n";
		#define CALL_SCOPED_CALLBACK(Name, ReturnType, Arguments, Body) \
			RUN_TEST(Name, function<ReturnType Arguments>, tc::jst::lambda([]Arguments noexcept -> ReturnType Body));
		FOR_ALL_CALLBACKS(CALL_SCOPED_CALLBACK)
		#undef CALL_SCOPED_CALLBACK
	}
	{
		std::cout << "===== TC_JS_MEMBER_FUNCTION =====\n";
		#define DEFINE_MEMBER(Name, ReturnType, Arguments, Body) TC_JS_MEMBER_FUNCTION(TestClass, m_jsfn##Name, ReturnType, Arguments) Body
		struct TestClass {
			FOR_ALL_CALLBACKS(DEFINE_MEMBER)
		} obj;
		#undef DEFINE_MEMBER
		#define CALL_MEMBER(Name, ReturnType, Arguments, Body) \
			RUN_TEST(Name, function<ReturnType Arguments>, obj.m_jsfn##Name);
		FOR_ALL_CALLBACKS(CALL_MEMBER)
		#undef CALL_MEMBER
	}
	{
		std::cout << "Calling callbacks through function\n";
		auto cbStorage = tc::jst::lambda([](string const str) noexcept {
			return string(tc::concat("hello ", tc::explicit_cast<std::string>(str)));
		});
		ensureJsFunctionDeduction<function<string(string const)>>(cbStorage);
		function<string(string const)> cb = cbStorage;
		_ASSERTEQUAL(tc::explicit_cast<std::string>(cb(string("world"))), "hello world");
	}
	{
		std::cout << "Passing callbacks as function parameter with temporary lambda\n";
		auto test = [](function<string(string)> cb) {
			_ASSERTEQUAL(tc::explicit_cast<std::string>(cb(string("world"))), "hello world");
		};
		test(tc::jst::lambda([](string const str) noexcept {
			return string(tc::concat("hello ", tc::explicit_cast<std::string>(str)));
		}));
	}
	return 0;
}
