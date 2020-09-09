#include "../../precompiled.h"
#include "MyLib.d.h"

int main() {
    static_assert(std::is_same<decltype(std::declval<tc::js::Symbol2>()->bar()), tc::jst::js_union<tc::js::Bar, tc::jst::js_undefined>>::value);
    static_assert(std::is_same<decltype(std::declval<tc::js::Symbol2>()->foo()), tc::jst::js_union<tc::js::Foo, tc::jst::js_undefined>>::value);
    static_assert(std::is_same<decltype(std::declval<tc::js::Symbol2>()->member()), tc::jst::js_string>::value);
    return 0;
}