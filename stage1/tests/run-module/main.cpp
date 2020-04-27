#include "../../precompiled.h"
#include "MyLib.d.h"

int main() {
	_ASSERTEQUAL(tc::js::MyLib::sumOfTwo(10, 20), 30);

	_ASSERTEQUAL(tc::js::MyLib::initially10(), 10);
	tc::js::MyLib::initially10(12);
	_ASSERTEQUAL(tc::js::MyLib::initially10(), 12);

	tc::js::MyLib::SomeObject obj(tc::js_types::create_js_object);
	_ASSERTEQUAL(tc::explicit_cast<std::string>(obj->str()), "");
	_ASSERTEQUAL(obj->arr()->length(), 0);
	_ASSERT(tc::empty(obj->arr()));
	_ASSERTEQUAL(tc::explicit_cast<std::string>(obj->foo(10)), "foo() retval number 10");

	tc::js::MyLib::SomeBaseClass objBase = obj;
	_ASSERT(objBase.getEmval().strictlyEquals(obj.getEmval()));
	_ASSERTEQUAL(tc::explicit_cast<std::string>(objBase->foo(10)), "foo() retval number 10");

	static_assert(std::is_enum<tc::js::MyLib::SomeEnum>::value);
	static_assert(10 == static_cast<int>(tc::js::MyLib::SomeEnum::ENUM10));
	static_assert(20 == static_cast<int>(tc::js::MyLib::SomeEnum::ENUM20));
	static_assert(21 == static_cast<int>(tc::js::MyLib::SomeEnum::ENUM21));

	_ASSERTEQUAL(tc::js::MyLib::SomeModule::sumOfThree(10, 20, 30), 60);

	tc::js::MyLib::SomeObject obj2 = tc::js::MyLib::createObject(tc::js_types::js_string("foobar"));
	_ASSERTEQUAL(tc::explicit_cast<std::string>(obj2->str()), "foobar");
	_ASSERTEQUAL(obj2->arr()->length(), 0);
	_ASSERT(tc::empty(obj2->arr()));

	tc::js::MyLib::SomeObject obj3 = tc::js::MyLib::ForwardReferencingNamespace::createSomeObject();
	_ASSERTEQUAL(tc::explicit_cast<std::string>(obj3->str()), "foobar");
	_ASSERTEQUAL(obj3->arr()->length(), 0);
	_ASSERT(tc::empty(obj3->arr()));

	tc::js::MyLib::double_u_uunderscore();

	_ASSERTEQUAL(tc::js::MyLib::runCallback(tc::js_types::js_lambda_wrap([](double a, double b) noexcept {
		_ASSERTEQUAL(a, 20);
		_ASSERTEQUAL(b, 30);
		return a + b;
	}), 20, 30), 50);
	printf("Success!\n");
}
