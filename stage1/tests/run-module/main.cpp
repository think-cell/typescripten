#include "../../precompiled.h"
#include "my-lib.d.h"

int main() {
	tc::js::my_lib::SomeObject obj;
	_ASSERTEQUAL("", tc::explicit_cast<std::string>(obj->str()));
	_ASSERTEQUAL(0, obj->arr()->length());
	_ASSERT(tc::empty(obj->arr()));
}
