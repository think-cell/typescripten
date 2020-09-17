#include "../../precompiled.h"
#include "MyLib.d.h"

int main() {
    // X is both a type alias and a class
    static_assert(std::is_same<tc::js::X_alias, tc::jst::js_string>::value);

    // X::Z declarations have been merged correctly
    static_assert(std::is_same<decltype(std::declval<tc::js::X_class::Z const&>()->b()), double>::value);
    static_assert(std::is_same<decltype(std::declval<tc::js::X_class::Z const&>()->d()), double>::value);
    static_assert(std::is_same<decltype(std::declval<tc::js::X_class::Z::C const&>()->c()), double>::value);

    // Duplicate declarations resolved inside another class
    static_assert(std::is_same<decltype(std::declval<tc::js::X_class::A_class::D const&>()->e()), double>::value);
    static_assert(std::is_same<tc::js::X_class::A_alias, double>::value);

    // Namespace + enum 
    static_assert(std::is_same<decltype(tc::js::XYZ_class::x()), double>::value);
    static_assert(static_cast<int>(tc::js::XYZ_enum::d) == 4);

    // Namespace + function
    static_assert(std::is_same<decltype(tc::js::test_class::T()), double>::value);
    static_assert(std::is_same<decltype(tc::js::test(std::declval<double>())), void>::value);
    static_assert(std::is_same<decltype(tc::js::test(std::declval<tc::jst::js_string>())), void>::value);
}
