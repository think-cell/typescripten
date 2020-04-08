#include "../../precompiled.h"
#include "my-lib.d.h"

int main() {
	tc::js::my_lib::SomeObject obj;
	_ASSERTEQUAL("", tc::explicit_cast<std::string>(obj->str()));
	_ASSERTEQUAL(0, obj->arr()->length());
	_ASSERT(tc::empty(obj->arr()));

	tc::js::my_lib::SomeObject obj2 = tc::js::my_lib::createObject(tc::js::js_string("foobar"));
	_ASSERTEQUAL("foobar", tc::explicit_cast<std::string>(obj2->str()));
	_ASSERTEQUAL(0, obj2->arr()->length());
	_ASSERT(tc::empty(obj2->arr()));

	_ASSERTEQUAL(30, tc::js::my_lib::sumOfTwo(10, 20));
}
