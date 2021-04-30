#include "../../precompiled.h"
#include "MyLib.d.h"

#define TEST(fn) \
    template< \
        typename R,  \
        typename ...Args \
    > \
    void fn ## _TestInvalidArgs(char) noexcept { \
    } \
    \
    template< \
        typename R,  \
        typename ...Args,  \
        std::enable_if_t<std::is_same<decltype(std::declval<tc::js::Foo>()-> fn (std::declval<Args>()...)), R>::value>* = nullptr \
    > \
    void fn ## _TestInvalidArgs(int) noexcept; \
    \
    template< \
        typename R,  \
        typename ...Args \
    > \
    void fn ## _TestValidArgs(char) noexcept; \
    \
    template< \
        typename R,  \
        typename ...Args,  \
        std::enable_if_t<std::is_same<decltype(std::declval<tc::js::Foo>()-> fn (std::declval<Args>()...)), R>::value>* = nullptr \
    > \
    void fn ## _TestValidArgs(int) noexcept {}

TEST(do_foo)
TEST(do_foo4)

int main() {
    do_foo_TestInvalidArgs<tc::js::string>(0);
    do_foo_TestValidArgs<tc::js::string, tc::js::string>(0);
    do_foo_TestInvalidArgs<tc::js::string, tc::js::undefined>(0);
    do_foo_TestInvalidArgs<tc::js::string, tc::jst::js_unknown>(0);
    do_foo_TestInvalidArgs<tc::js::string, double>(0);
    
    do_foo_TestValidArgs<tc::js::string, tc::js::string, double>(0);
    do_foo_TestValidArgs<tc::js::string, tc::js::string, tc::js::undefined>(0);

    do_foo_TestInvalidArgs<tc::js::string, tc::js::string, tc::js::string>(0);
    do_foo_TestInvalidArgs<tc::js::string, tc::js::string, tc::jst::js_unknown>(0);
    do_foo_TestInvalidArgs<tc::js::string, tc::js::undefined, double>(0);

    do_foo4_TestValidArgs<tc::js::string, tc::js::string, double>(0);
    do_foo4_TestValidArgs<tc::js::string, tc::js::undefined, double>(0);
    do_foo4_TestValidArgs<tc::js::string, tc::js::string, double, tc::js::string>(0);
    do_foo4_TestValidArgs<tc::js::string, tc::js::string, double, tc::js::undefined>(0);
    
    return 0;
}