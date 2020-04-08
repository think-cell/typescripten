#include "../../precompiled.h"
#include "MyLib.d.h"

int main() {
	tc::js::MyLib::SomeObject obj;
	_ASSERTEQUAL("", tc::explicit_cast<std::string>(obj->str()));
	_ASSERTEQUAL(0, obj->arr()->length());
	_ASSERT(tc::empty(obj->arr()));

	tc::js::MyLib::SomeObject obj2 = tc::js::MyLib::createObject(tc::js::js_string("foobar"));
	_ASSERTEQUAL("foobar", tc::explicit_cast<std::string>(obj2->str()));
	_ASSERTEQUAL(0, obj2->arr()->length());
	_ASSERT(tc::empty(obj2->arr()));

	_ASSERTEQUAL(30, tc::js::MyLib::sumOfTwo(10, 20));
}
