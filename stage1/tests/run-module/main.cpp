#include "../../precompiled.h"
#include "my-lib.d.h"

using SomeObject = tc::js::_jsall::_js_jMyLib_dSomeObject;

int main() {
	SomeObject obj;
	_ASSERTEQUAL("", tc::explicit_cast<std::string>(obj->str()));
	_ASSERTEQUAL(0, obj->arr()->length());
	_ASSERT(tc::empty(obj->arr()));
}
