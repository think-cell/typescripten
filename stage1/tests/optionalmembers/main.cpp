#include "../../precompiled.h"
#include "MyLib.d.h"

int main() {
    static_assert(std::is_same<decltype(std::declval<tc::js::Symbol2>()->bar()), tc::jst::union_t<tc::js::Bar, tc::js::undefined>>::value);
    static_assert(std::is_same<decltype(std::declval<tc::js::Symbol2>()->foo()), tc::jst::union_t<tc::js::Foo, tc::js::undefined>>::value);
    static_assert(std::is_same<decltype(std::declval<tc::js::Symbol2>()->member()), tc::js::string>::value);
    return 0;
}