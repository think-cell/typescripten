#include <emscripten/val.h>
#include <type_traits>
#include "type_traits.h"
#include "range_defines.h"
#include "js_callback.h"

using emscripten::val;
using tc::js::js_ref;
using tc::js::IJsFunction;
using tc::js::pass_this_t;
using tc::js::pass_all_arguments_t;

struct SomeJsClass : virtual tc::js::IUnknown {
    auto intValue() { return _getProperty<int>("intValue"); }

    static auto _construct(int val) {
        return val::module_property("SomeJsClass").new_(val);
    }
};

#define FOR_ALL_CALLBACKS(CreateCallback) \
    CreateCallback(TestSum, int, (int a, int b), { \
        _ASSERTEQUAL(a, 10); \
        _ASSERTEQUAL(b, 20); \
        return a + b; \
    }) \
    CreateCallback(TestPrint, void, (std::string sMessage), { \
        _ASSERTEQUAL(sMessage, "hello"); \
    })\
    CreateCallback(TestPassAllArguments, void, (pass_all_arguments_t, val emvalArgs, int a), { \
        _ASSERTEQUAL(a, 1); \
        _ASSERTEQUAL(emvalArgs[1].as<std::string>(), "message"); \
        _ASSERTEQUAL(emvalArgs["length"].as<int>(), 3); \
    }) \
    CreateCallback(TestPassThis, void, (pass_this_t, val emvalThis, int a, std::string b, val c), { \
        _ASSERTEQUAL(emvalThis["secretField"].as<std::string>(), "secretValue"); \
        _ASSERTEQUAL(a, 1); \
        _ASSERTEQUAL(b, "message"); \
        _ASSERT(c.isNull()); \
    }) \
    CreateCallback(TestPassThisPassAllArguments, void, (pass_this_t, val emvalThis, pass_all_arguments_t, val emvalArgs, int a), { \
        _ASSERTEQUAL(emvalThis["secretField"].as<std::string>(), "secretValue"); \
        _ASSERTEQUAL(a, 1); \
        _ASSERTEQUAL(emvalArgs[1].as<std::string>(), "message"); \
        _ASSERTEQUAL(emvalArgs["length"].as<int>(), 3); \
    }) \
    CreateCallback(TestPassAllArgumentsAndReturn, js_ref<SomeJsClass>, (pass_all_arguments_t, val emvalArgs, int a), { \
        _ASSERTEQUAL(a, 1); \
        _ASSERTEQUAL(emvalArgs[1].as<std::string>(), "message"); \
        _ASSERTEQUAL(emvalArgs["length"].as<int>(), 3); \
        return js_ref<SomeJsClass>(123); \
    }) \
    CreateCallback(TestPassThisAndReturn, js_ref<SomeJsClass>, (pass_this_t, val emvalThis, int a, std::string b, val c), { \
        _ASSERTEQUAL(emvalThis["secretField"].as<std::string>(), "secretValue"); \
        _ASSERTEQUAL(a, 1); \
        _ASSERTEQUAL(b, "message"); \
        _ASSERT(c.isNull()); \
        return js_ref<SomeJsClass>(123); \
    }) \
    CreateCallback(TestPassThisPassAllArgumentsAndReturn, js_ref<SomeJsClass>, (pass_this_t, val emvalThis, pass_all_arguments_t, val emvalArgs, int a), { \
        _ASSERTEQUAL(emvalThis["secretField"].as<std::string>(), "secretValue"); \
        _ASSERTEQUAL(a, 1); \
        _ASSERTEQUAL(emvalArgs[1].as<std::string>(), "message"); \
        _ASSERTEQUAL(emvalArgs["length"].as<int>(), 3); \
        return js_ref<SomeJsClass>(123); \
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
        val::module_property(#Name)(std::forward<Fn>(cb)); \
        printf(#Name " end\n"); \
    }

int main() {
    {
        printf("===== js_lambda_wrap =====\n");
        #define CALL_SCOPED_CALLBACK(Name, ReturnType, Arguments, Body) \
            RUN_TEST(Name, js_ref<IJsFunction<ReturnType Arguments>>, tc::js::js_lambda_wrap([]Arguments noexcept -> ReturnType Body));
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
            RUN_TEST(Name, js_ref<IJsFunction<ReturnType Arguments>>, obj.m_jsfn##Name);
        FOR_ALL_CALLBACKS(CALL_MEMBER)
        #undef CALL_MEMBER
    }
    {
        printf("Calling callbacks through js_ref\n");
        tc::js::js_lambda_wrap cbStorage([](std::string str) noexcept {
            return std::string("hello " + str);
        });
        js_ref<IJsFunction<std::string(std::string)>> cb = cbStorage;
       _ASSERTEQUAL(cb("world"), "hello world");
    }
    return 0;
}
