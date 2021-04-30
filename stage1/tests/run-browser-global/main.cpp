#include "../../precompiled.h"
#include "MyLib.d.h"

int main() {
	_ASSERTEQUAL(tc::js::sumOfTwo(10, 20), 30);

	_ASSERTEQUAL(tc::js::initially10(), 10);
	tc::js::initially10(12);
	_ASSERTEQUAL(tc::js::initially10(), 12);

	tc::js::SomeObject obj(tc::jst::create_js_object);
	_ASSERTEQUAL(tc::explicit_cast<std::string>(obj->str()), "");
	_ASSERTEQUAL(obj->arr()->length(), 0);
	_ASSERT(tc::empty(obj->arr()));
	_ASSERTEQUAL(tc::explicit_cast<std::string>(obj->foo(10)), "foo() retval number 10");

	tc::js::SomeBaseClass objBase = obj;
	_ASSERT(objBase.getEmval().strictlyEquals(obj.getEmval()));
	_ASSERTEQUAL(tc::explicit_cast<std::string>(objBase->foo(10)), "foo() retval number 10");

	static_assert(std::is_enum<tc::js::SomeEnum>::value);
	static_assert(10 == static_cast<int>(tc::js::SomeEnum::ENUM10));
	static_assert(20 == static_cast<int>(tc::js::SomeEnum::ENUM20));
	static_assert(21 == static_cast<int>(tc::js::SomeEnum::ENUM21));

	_ASSERTEQUAL(tc::js::SomeModule::sumOfThree(10, 20, 30), 60);

	tc::js::SomeObject obj2 = tc::js::createObject(tc::js::string("foobar"));
	_ASSERTEQUAL(tc::explicit_cast<std::string>(obj2->str()), "foobar");
	_ASSERTEQUAL(obj2->arr()->length(), 0);
	_ASSERT(tc::empty(obj2->arr()));

	tc::js::SomeObject obj3 = tc::js::appendToObjectStr(obj2);
	_ASSERT(obj3.getEmval().strictlyEquals(obj2.getEmval()));
	_ASSERTEQUAL(tc::explicit_cast<std::string>(obj3->str()), "foobarx");
	_ASSERTEQUAL(obj3->arr()->length(), 0);
	_ASSERT(tc::empty(obj3->arr()));

	tc::js::MyInterface iface = obj3;

	printf("Success!\n");
}
