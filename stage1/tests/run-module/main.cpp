#include "../../precompiled.h"
#include "MyLib.d.h"

int main() {
	_ASSERTEQUAL(tc::js::MyLib::sumOfTwo(10, 20), 30);

	_ASSERTEQUAL(tc::js::MyLib::initially10(), 10);
	tc::js::MyLib::initially10(12);
	_ASSERTEQUAL(tc::js::MyLib::initially10(), 12);

	tc::js::MyLib::SomeObject obj(tc::jst::create_js_object);
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

	static_assert(std::is_enum<tc::js::MyLib::SomeHeterogeneousEnum>::value);
	tc::js::MyLib::checkHeterogeneousEnum(
		tc::js::MyLib::SomeHeterogeneousEnum::ENUM30,
		tc::js::MyLib::SomeHeterogeneousEnum::ENUMFoo,
		tc::js::MyLib::SomeHeterogeneousEnum::ENUM40
	);
	_ASSERTEQUAL(tc::js::MyLib::getEnum30(), tc::js::MyLib::SomeHeterogeneousEnum::ENUM30);
	_ASSERTEQUAL(tc::js::MyLib::getEnumFoo(), tc::js::MyLib::SomeHeterogeneousEnum::ENUMFoo);
	_ASSERTEQUAL(tc::js::MyLib::getEnum40(), tc::js::MyLib::SomeHeterogeneousEnum::ENUM40);

	_ASSERTEQUAL(tc::js::MyLib::SomeModule::sumOfThree(10, 20, 30), 60);

	tc::js::MyLib::SomeObject obj2 = tc::js::MyLib::createObject(tc::jst::js_string("foobar"));
	_ASSERTEQUAL(tc::explicit_cast<std::string>(obj2->str()), "foobar");
	_ASSERTEQUAL(obj2->arr()->length(), 0);
	_ASSERT(tc::empty(obj2->arr()));

	tc::js::MyLib::SomeObject obj3 = tc::js::MyLib::ForwardReferencingNamespace::createSomeObject();
	_ASSERTEQUAL(tc::explicit_cast<std::string>(obj3->str()), "foobar");
	_ASSERTEQUAL(obj3->arr()->length(), 0);
	_ASSERT(tc::empty(obj3->arr()));

	tc::js::MyLib::double_u_uunderscore();

	_ASSERTEQUAL(tc::js::MyLib::runCallback(tc::jst::js_lambda_wrap([](double a, double b) noexcept {
		_ASSERTEQUAL(a, 20);
		_ASSERTEQUAL(b, 30);
		return a + b;
	}), 20, 30), 50);

	{
		auto impl = tc::js::MyLib::MyInterfaceImpl2(tc::jst::create_js_object);
		tc::js::MyLib::MyInterface1 int1 = impl;
		tc::js::MyLib::MyInterface2 int2 = impl;
		tc::js::MyLib::MyInterface3 int3 = impl;
		tc::js::MyLib::MyInterface4 int4 = impl;
		tc::js::MyLib::MyInterface5 int5 = impl;
		tc::js::MyLib::MyInterface6 int6 = impl;
		_ASSERTEQUAL(tc::explicit_cast<std::string>(int1->func1()), "func1");
		_ASSERTEQUAL(tc::explicit_cast<std::string>(int2->func2()), "func2");
		_ASSERTEQUAL(tc::explicit_cast<std::string>(int3->func3()), "func3");
		_ASSERTEQUAL(tc::explicit_cast<std::string>(int4->func4()), "func4");
		_ASSERTEQUAL(tc::explicit_cast<std::string>(int5->func5()), "func5");
		_ASSERTEQUAL(tc::explicit_cast<std::string>(int6->func6()), "func6");
	}

	_ASSERTEQUAL(tc::js::MyLib::literalTypesFunction(10, tc::jst::js_string("str")), 20);

	printf("Success!\n");
}
