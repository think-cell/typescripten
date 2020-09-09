#include "../../precompiled.h"
#include "MyLib.d.h"

int main() {
	static_assert(std::is_same<tc::js::GlobalFooBar, tc::jst::js_union<tc::js::test2::A, tc::js::test::B>>::value);
	static_assert(std::is_same<tc::js::GlobalFn, tc::jst::js_function<tc::js::test::FooBar(tc::js::test2::FooBar3)>>::value);

	static_assert(std::is_same<tc::js::test2::FooBar3, tc::jst::js_union<tc::js::test2::A, double>>::value);
	static_assert(std::is_same<tc::js::test::FooBar, tc::jst::js_union<tc::js::test::SBar, tc::js::test::SFoo>>::value);
	static_assert(std::is_same<tc::js::test::FooBar, tc::js::test2::FooBar2>::value);
	static_assert(std::is_same<tc::js::test::FooBar, tc::js::test2::FooBar>::value);
}
