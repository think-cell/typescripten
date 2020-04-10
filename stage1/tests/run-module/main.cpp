#include "../../precompiled.h"
#include "MyLib.d.h"

int main() {
	_ASSERTEQUAL(30, tc::js::MyLib::sumOfTwo(10, 20));

	_ASSERTEQUAL(10, tc::js::MyLib::initially10());
	tc::js::MyLib::initially10(12);
	_ASSERTEQUAL(12, tc::js::MyLib::initially10());

	tc::js::MyLib::SomeObject obj;
	_ASSERTEQUAL("", tc::explicit_cast<std::string>(obj->str()));
	_ASSERTEQUAL(0, obj->arr()->length());
	_ASSERT(tc::empty(obj->arr()));
	_ASSERTEQUAL("foo() retval number 10", tc::explicit_cast<std::string>(obj->foo(10)));

	tc::js::MyLib::SomeBaseClass objBase = obj;
	_ASSERT(objBase.getEmval().strictlyEquals(obj.getEmval()));
	_ASSERTEQUAL("foo() retval number 10", tc::explicit_cast<std::string>(objBase->foo(10)));

	static_assert(std::is_enum<tc::js::MyLib::SomeEnum>::value);
	static_assert(10 == static_cast<int>(tc::js::MyLib::SomeEnum::ENUM10));
	static_assert(20 == static_cast<int>(tc::js::MyLib::SomeEnum::ENUM20));
	static_assert(21 == static_cast<int>(tc::js::MyLib::SomeEnum::ENUM21));

	_ASSERTEQUAL(60, tc::js::MyLib::SomeModule::sumOfThree(10, 20, 30));

	tc::js::MyLib::SomeObject obj2 = tc::js::MyLib::createObject(tc::js::js_string("foobar"));
	_ASSERTEQUAL("foobar", tc::explicit_cast<std::string>(obj2->str()));
	_ASSERTEQUAL(0, obj2->arr()->length());
	_ASSERT(tc::empty(obj2->arr()));
}
