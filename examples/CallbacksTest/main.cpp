#include <emscripten/val.h>
#include <type_traits>
#include "type_traits.h"
#include "range.h"
#include "range_defines.h"
#include "js_callback.h"
#include "js_bootstrap.h"

using tc::js::js_unknown;
using tc::js::js_function;
using tc::js::pass_this_t;
using tc::js::pass_all_arguments_t;
using tc::js::globals::Array;

struct _js_SomeJsClass : virtual tc::js::IUnknown {
    auto intValue() { return _getProperty<int>("intValue"); }

    static auto _construct(int v) {
        return emscripten::val::module_property("SomeJsClass").new_(v);
    }
};
using SomeJsClass = tc::js::js_ref<_js_SomeJsClass>;

#define FOR_ALL_CALLBACKS(CreateCallback) \
    CreateCallback(TestSum, int, (int a, int b), { \
        _ASSERTEQUAL(a, 10); \
        _ASSERTEQUAL(b, 20); \
        return a + b; \
    }) \
    CreateCallback(TestPrint, void, (tc::js::globals::String sMessage), { \
        _ASSERTEQUAL(std::string(sMessage), "hello"); \
    })\
    CreateCallback(TestPassAllArguments, void, (pass_all_arguments_t, Array<js_unknown> jsarrunkArgs, int a), { \
        _ASSERTEQUAL(a, 1); \
        _ASSERTEQUAL(std::string(tc::js::globals::String(jsarrunkArgs[1])), "message"); \
        _ASSERTEQUAL(jsarrunkArgs->length(), 3); \
    }) \
    CreateCallback(TestPassThis, void, (pass_this_t, SomeJsClass jssjcThis, int a, tc::js::globals::String b, std::optional<js_unknown> c), { \
        _ASSERTEQUAL(jssjcThis->intValue(), 10); \
        _ASSERTEQUAL(a, 1); \
        _ASSERTEQUAL(std::string(b), "message"); \
        _ASSERT(!c.has_value()); \
    }) \
    CreateCallback(TestPassThisPassAllArguments, void, (pass_this_t, SomeJsClass jssjcThis, pass_all_arguments_t, Array<js_unknown> jsarrunkArgs, int a), { \
        _ASSERTEQUAL(jssjcThis->intValue(), 10); \
        _ASSERTEQUAL(a, 1); \
        _ASSERTEQUAL(std::string(tc::js::globals::String(jsarrunkArgs[1])), "message"); \
        _ASSERTEQUAL(jsarrunkArgs->length(), 3); \
    }) \
    CreateCallback(TestPassAllArgumentsAndReturn, SomeJsClass, (pass_all_arguments_t, Array<js_unknown> jsarrunkArgs, int a), { \
        _ASSERTEQUAL(a, 1); \
        _ASSERTEQUAL(std::string(tc::js::globals::String(jsarrunkArgs[1])), "message"); \
        _ASSERTEQUAL(jsarrunkArgs->length(), 3); \
        return SomeJsClass(123); \
    }) \
    CreateCallback(TestPassThisAndReturn, SomeJsClass, (pass_this_t, SomeJsClass jssjcThis, int a, tc::js::globals::String b, std::optional<js_unknown> c), { \
        _ASSERTEQUAL(jssjcThis->intValue(), 10); \
        _ASSERTEQUAL(a, 1); \
        _ASSERTEQUAL(std::string(b), "message"); \
        _ASSERT(!c.has_value()); \
        return SomeJsClass(123); \
    }) \
    CreateCallback(TestPassThisPassAllArgumentsAndReturn, SomeJsClass, (pass_this_t, SomeJsClass jssjcThis, pass_all_arguments_t, Array<js_unknown> jsarrunkArgs, int a), { \
        _ASSERTEQUAL(jssjcThis->intValue(), 10); \
        _ASSERTEQUAL(a, 1); \
        _ASSERTEQUAL(std::string(tc::js::globals::String(jsarrunkArgs[1])), "message"); \
        _ASSERTEQUAL(jsarrunkArgs->length(), 3); \
        return SomeJsClass(123); \
    })

// Macro so callback creation is performed between "start" and "end".
// As ExpectedType is wrapped into parenthesis (to templates can be passed),
// we prepend 'void' to make it a function type.
#define RUN_TEST(Name, ExpectedType, Callback) \
    { \
        printf(#Name " start\n"); \
        auto&& cb = Callback; \
        using Fn = decltype(cb); \
        static_assert(std::is_convertible< \
            tc::remove_cvref_t<Fn>*, \
            ExpectedType* \
        >::value, #Name); \
        emscripten::val::module_property(#Name)(std::forward<Fn>(cb)); \
        printf(#Name " end\n"); \
    }

int main() {
    {
        printf("===== js_lambda_wrap =====\n");
        #define CALL_SCOPED_CALLBACK(Name, ReturnType, Arguments, Body) \
            RUN_TEST(Name, js_function<ReturnType Arguments>, tc::js::js_lambda_wrap([]Arguments noexcept -> ReturnType Body));
        FOR_ALL_CALLBACKS(CALL_SCOPED_CALLBACK)
        #undef CALL_SCOPED_CALLBACK
    }
    {
        printf("===== TC_JS_MEMBER_FUNCTION =====\n");
        #define DEFINE_MEMBER(Name, ReturnType, Arguments, Body) TC_JS_MEMBER_FUNCTION(TestClass, m_jsfn##Name, ReturnType, Arguments) Body
        struct TestClass {
            FOR_ALL_CALLBACKS(DEFINE_MEMBER)
        } obj;
        #undef DEFINE_MEMBER
        #define CALL_MEMBER(Name, ReturnType, Arguments, Body) \
            RUN_TEST(Name, js_function<ReturnType Arguments>, obj.m_jsfn##Name);
        FOR_ALL_CALLBACKS(CALL_MEMBER)
        #undef CALL_MEMBER
    }
    {
        printf("Calling callbacks through js_function\n");
        tc::js::js_lambda_wrap cbStorage([](tc::js::globals::String str) noexcept {
            return tc::js::globals::String(tc::concat("hello ", std::string(str)));
        });
        js_function<tc::js::globals::String(tc::js::globals::String)> cb = cbStorage;
       _ASSERTEQUAL(std::string(cb(tc::js::globals::String("world"))), "hello world");
    }
    return 0;
}
