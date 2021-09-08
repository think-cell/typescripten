#include <emscripten/val.h>
#include <iostream>
#include <type_traits>
#include "explicit_cast.h"
#include "type_traits.h"
#include "assert_defs.h"
#include "js_callback.h"
#include "js_bootstrap.h"

using tc::js::Promise;
using tc::js::null;
using tc::jst::js_lambda_wrap;
using tc::js::string;
using tc::js::undefined;
using tc::jst::js_union;
using tc::js::any;

auto SuccessfulPromise(double x) {
	return static_cast<Promise<double>>(emscripten::val::module_property("successfulPromise")(x));
}

auto FailingPromise(string x) {
	return static_cast<Promise<double>>(emscripten::val::module_property("failingPromise")(x));
}

void CompletePromiseTest() {
	// We should ensure that all callbacks are correctly called.
	emscripten::val::module_property("completePromiseTest")();
}

int main() {
	std::cout << "Should call 3 tests (order is arbitrary)\n";
	{
		Promise<double> p1 = SuccessfulPromise(10.0);

		static auto l1 = js_lambda_wrap([](double x) noexcept {
			_ASSERTEQUAL(x, 10);
			return 11.0;
		});
		Promise<double> p2 = p1->then(l1);

		static auto l2 = js_lambda_wrap([](double x) noexcept {
			_ASSERTEQUAL(x, 11);
			return FailingPromise(string("err"));
		});
		Promise<double> p3 = p2->then(l2);

		static auto l3ok = js_lambda_wrap([](double x) noexcept -> string {
			_ASSERTEQUAL(x, 20);
			_ASSERTFALSE;
		});
		static auto l3fail = js_lambda_wrap([](any x) noexcept {
			_ASSERTEQUAL(tc::explicit_cast<std::string>(tc::explicit_cast<string>(x)), "err");
			return string("hello");
		});
		Promise<string> p4 = p3->then(l3ok, l3fail);

		static auto l4 = js_lambda_wrap([](string x) noexcept {
			_ASSERTEQUAL(tc::explicit_cast<std::string>(x), "hello");
		});
		Promise<void> p5 = p4->then(l4);

		static auto l5 = js_lambda_wrap([](undefined) noexcept {
			std::cout << "Promise 1/3 success\n";
			CompletePromiseTest();
		});
		p5->then(l5);
	}
	{
		Promise<double> p1 = SuccessfulPromise(10.0);

		static auto l1ok = js_lambda_wrap([](double x) noexcept {
			_ASSERTEQUAL(x, 10);
			return string("ok");
		});
		static auto l1fail = js_lambda_wrap([](any) noexcept {
			_ASSERTFALSE;
			return null();
		});
		Promise<js_union<string, null>> p2 = p1->then(l1ok, l1fail);

		static auto l2 = js_lambda_wrap([](js_union<string, null> x) noexcept {
			_ASSERTEQUAL(tc::explicit_cast<std::string>(tc::explicit_cast<string>(x)), "ok");
			std::cout << "Promise 2/3 success\n";
			CompletePromiseTest();
		});
		p2->then(l2);
	}
	{
		Promise<double> p1 = FailingPromise(string("err"));

		static auto l1ok = js_lambda_wrap([](double) noexcept {
			_ASSERTFALSE;
			return string("ok");
		});
		static auto l1fail = js_lambda_wrap([](any x) noexcept {
			_ASSERTEQUAL(tc::explicit_cast<std::string>(tc::explicit_cast<string>(x)), "err");
			return null();
		});
		Promise<js_union<string, null>> p2 = p1->then(l1ok, l1fail);

		static auto l2 = js_lambda_wrap([](js_union<string, null> x) noexcept {
			static_cast<void>(static_cast<null>(x));
			std::cout << "Promise 3/3 success\n";
			CompletePromiseTest();
		});
		p2->then(l2);
	}
	return 0;
}
