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

#define FOR_ALL_CALLBACKS(CreateCallback) \
    CreateCallback(TestSum, (js_ref<IJsFunction<int(int, int)>>), int, (int a, int b), { \
        _ASSERTEQUAL(a, 10); \
        _ASSERTEQUAL(b, 20); \
        return a + b; \
    }) \
    CreateCallback(TestPrint, (js_ref<IJsFunction<void(std::string)>>), void, (std::string sMessage), { \
        _ASSERTEQUAL(sMessage, "hello"); \
    })\
    CreateCallback(TestPassAllArguments, (js_ref<IJsFunction<void(pass_all_arguments_t, val, int)>>), void, (pass_all_arguments_t, val emvalArgs, int a), { \
        _ASSERTEQUAL(a, 1); \
        _ASSERTEQUAL(emvalArgs[1].as<std::string>(), "message"); \
        _ASSERTEQUAL(emvalArgs["length"].as<int>(), 3); \
    }) \
    CreateCallback(TestPassThis, (js_ref<IJsFunction<void(pass_this_t, val, int, std::string, val)>>), void, (pass_this_t, val emvalThis, int a, std::string b, val c), { \
        _ASSERTEQUAL(emvalThis["secretField"].as<std::string>(), "secretValue"); \
        _ASSERTEQUAL(a, 1); \
        _ASSERTEQUAL(b, "message"); \
        _ASSERT(c.isNull()); \
    }) \
    CreateCallback(TestPassThisPassAllArguments, (js_ref<IJsFunction<void(pass_this_t, val, pass_all_arguments_t, val, int)>>), void, (pass_this_t, val emvalThis, pass_all_arguments_t, val emvalArgs, int a), { \
        _ASSERTEQUAL(emvalThis["secretField"].as<std::string>(), "secretValue"); \
        _ASSERTEQUAL(a, 1); \
        _ASSERTEQUAL(emvalArgs[1].as<std::string>(), "message"); \
        _ASSERTEQUAL(emvalArgs["length"].as<int>(), 3); \
    }) \
    CreateCallback(TestPassAllArgumentsAndReturn, (js_ref<IJsFunction<std::string(pass_all_arguments_t, val, int)>>), std::string, (pass_all_arguments_t, val emvalArgs, int a), { \
        _ASSERTEQUAL(a, 1); \
        _ASSERTEQUAL(emvalArgs[1].as<std::string>(), "message"); \
        _ASSERTEQUAL(emvalArgs["length"].as<int>(), 3); \
        return "returnValue"; \
    }) \
    CreateCallback(TestPassThisAndReturn, (js_ref<IJsFunction<std::string(pass_this_t, val, int, std::string, val)>>), std::string, (pass_this_t, val emvalThis, int a, std::string b, val c), { \
        _ASSERTEQUAL(emvalThis["secretField"].as<std::string>(), "secretValue"); \
        _ASSERTEQUAL(a, 1); \
        _ASSERTEQUAL(b, "message"); \
        _ASSERT(c.isNull()); \
        return "returnValue"; \
    }) \
    CreateCallback(TestPassThisPassAllArgumentsAndReturn, (js_ref<IJsFunction<std::string(pass_this_t, val, pass_all_arguments_t, val, int)>>), std::string, (pass_this_t, val emvalThis, pass_all_arguments_t, val emvalArgs, int a), { \
        _ASSERTEQUAL(emvalThis["secretField"].as<std::string>(), "secretValue"); \
        _ASSERTEQUAL(a, 1); \
        _ASSERTEQUAL(emvalArgs[1].as<std::string>(), "message"); \
        _ASSERTEQUAL(emvalArgs["length"].as<int>(), 3); \
        return "returnValue"; \
    })

template<typename>
struct UnwrapType {};

template<typename T>
struct UnwrapType<void(T)> {
    using type = T;
};

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
            typename UnwrapType<void ExpectedType>::type* \
        >::value, #Name); \
        val::module_property(#Name)(std::forward<Fn>(cb)); \
        printf(#Name " end\n"); \
    }

int main() {
    {
        printf("===== js_lambda_wrap =====\n");
        #define CALL_SCOPED_CALLBACK(Name, ExpectedType, ReturnType, Arguments, Body) \
            RUN_TEST(Name, ExpectedType, tc::js::js_lambda_wrap([]Arguments noexcept -> ReturnType Body));
        FOR_ALL_CALLBACKS(CALL_SCOPED_CALLBACK)
        #undef CALL_SCOPED_CALLBACK
    }
    {
        printf("===== TC_JS_MEMBER_FUNCTION =====\n");
        #define DEFINE_MEMBER(Name, ExpectedType, ReturnType, Arguments, Body) TC_JS_MEMBER_FUNCTION(TestClass, m_emval##Name, ReturnType, Arguments) Body
        struct TestClass {
            FOR_ALL_CALLBACKS(DEFINE_MEMBER)
        } obj;
        #undef DEFINE_MEMBER
        #define CALL_MEMBER(Name, ExpectedType, ReturnType, Arguments, Body) \
            RUN_TEST(Name, ExpectedType, obj.m_emval##Name);
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
