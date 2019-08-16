#include <emscripten/val.h>
#include <type_traits>
#include "type_traits.h"
#include "range_defines.h"
#include "js_bootstrap.h"
#include "js_callback.h"

using emscripten::val;
using tc::js::js_ref;
using tc::js::IJsFunction;

#define FOR_ALL_CALLBACKS(CreateCallback) \
    CreateCallback(TestSum, (js_ref<IJsFunction<int(int, int)>>), int, (int a, int b), { \
        _ASSERTEQUAL(a, 10); \
        _ASSERTEQUAL(b, 20); \
        return a + b; \
    }) \
    CreateCallback(TestPrint, (js_ref<IJsFunction<void(std::string)>>), void, (std::string sMessage), { \
        _ASSERTEQUAL(sMessage, "hello"); \
    })\
    CreateCallback(TestPassAllArguments, (js_ref<IJsFunction<void(int), void, val>>), void, (tc::js::pass_all_arguments_t, val emvalArgs, int a), { \
        _ASSERTEQUAL(a, 1); \
        _ASSERTEQUAL(emvalArgs[1].as<std::string>(), "message"); \
        _ASSERTEQUAL(emvalArgs["length"].as<int>(), 3); \
    }) \
    CreateCallback(TestPassThis, (js_ref<IJsFunction<void(int, std::string, val), val>>), void, (tc::js::pass_this_t, val emvalThis, int a, std::string b, val c), { \
        _ASSERTEQUAL(emvalThis["secretField"].as<std::string>(), "secretValue"); \
        _ASSERTEQUAL(a, 1); \
        _ASSERTEQUAL(b, "message"); \
        _ASSERT(c.isNull()); \
    }) \
    CreateCallback(TestPassThisPassAllArguments, (js_ref<IJsFunction<void(int), val, val>>), void, (tc::js::pass_this_t, val emvalThis, tc::js::pass_all_arguments_t, val emvalArgs, int a), { \
        _ASSERTEQUAL(emvalThis["secretField"].as<std::string>(), "secretValue"); \
        _ASSERTEQUAL(a, 1); \
        _ASSERTEQUAL(emvalArgs[1].as<std::string>(), "message"); \
        _ASSERTEQUAL(emvalArgs["length"].as<int>(), 3); \
    }) \
    CreateCallback(TestPassAllArgumentsAndReturn, (js_ref<IJsFunction<std::string(int), void, val>>), std::string, (tc::js::pass_all_arguments_t, val emvalArgs, int a), { \
        _ASSERTEQUAL(a, 1); \
        _ASSERTEQUAL(emvalArgs[1].as<std::string>(), "message"); \
        _ASSERTEQUAL(emvalArgs["length"].as<int>(), 3); \
        return "returnValue"; \
    }) \
    CreateCallback(TestPassThisAndReturn, (js_ref<IJsFunction<std::string(int, std::string, val), val>>), std::string, (tc::js::pass_this_t, val emvalThis, int a, std::string b, val c), { \
        _ASSERTEQUAL(emvalThis["secretField"].as<std::string>(), "secretValue"); \
        _ASSERTEQUAL(a, 1); \
        _ASSERTEQUAL(b, "message"); \
        _ASSERT(c.isNull()); \
        return "returnValue"; \
    }) \
    CreateCallback(TestPassThisPassAllArgumentsAndReturn, (js_ref<IJsFunction<std::string(int), val, val>>), std::string, (tc::js::pass_this_t, val emvalThis, tc::js::pass_all_arguments_t, val emvalArgs, int a), { \
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
        printf("===== CScopedCallback =====\n");
        #define CALL_SCOPED_CALLBACK(Name, ExpectedType, ReturnType, Arguments, Body) \
            RUN_TEST(Name, ExpectedType, tc::js::CScopedCallback([]Arguments noexcept -> ReturnType Body));
        FOR_ALL_CALLBACKS(CALL_SCOPED_CALLBACK)
        #undef CALL_SCOPED_CALLBACK
    }
    {
        printf("===== NewFiresOnceCallback =====\n");
        #define CALL_FIRES_ONCE_CALLBACK(Name, ExpectedType, ReturnType, Arguments, Body) \
            RUN_TEST(Name, ExpectedType, tc::js::NewFiresOnceCallback([]Arguments noexcept -> ReturnType Body));
        FOR_ALL_CALLBACKS(CALL_FIRES_ONCE_CALLBACK)
        #undef CALL_FIRES_ONCE_CALLBACK
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
        printf("===== CHeapCallback =====\n");
        RUN_TEST(TestHeapCallback, (js_ref<IJsFunction<void(int)>>), tc::js::NewHeapCallback([expectedX = 2](int x) mutable noexcept {
            _ASSERTEQUAL(x, expectedX);
            expectedX--;
            if (x == 0)
                return tc::js::DeleteThisCallback();
            else
                return tc::js::KeepThisCallback();
        }));
        RUN_TEST(TestHeapCallbackWithReturn, (js_ref<IJsFunction<int(int)>>), tc::js::NewHeapCallback([expectedX = 2](int x) mutable noexcept {
            _ASSERTEQUAL(x, expectedX);
            expectedX--;
            if (x == 0)
                return tc::js::DeleteThisCallback(x + 10);
            else
                return tc::js::KeepThisCallback(x + 10);
        }));
        RUN_TEST(TestHeapCallbackPassAllArguments, (js_ref<IJsFunction<void(int), void, val>>), tc::js::NewHeapCallback([expectedX = 2](tc::js::pass_all_arguments_t, val emvalArgs, int x) mutable noexcept {
            _ASSERTEQUAL(x, emvalArgs[0].as<int>());
            _ASSERTEQUAL(2, emvalArgs["length"].as<int>());
            _ASSERTEQUAL(x, expectedX);
            expectedX--;
            if (x == 0)
                return tc::js::DeleteThisCallback();
            else
                return tc::js::KeepThisCallback();
        }));
        RUN_TEST(TestHeapCallbackWithStringReturn, (js_ref<IJsFunction<std::string(int)>>), tc::js::NewHeapCallback([expectedX = 2](int x) mutable noexcept {
            _ASSERTEQUAL(x, expectedX);
            char buf[2] = { '1', static_cast<char>(x + '0') };
            expectedX--;
            if (x == 0)
                return tc::js::DeleteThisCallback<std::string>(buf, buf + 2);
            else
                return tc::js::KeepThisCallback<std::string>(buf, buf + 2);
        }));
    }
    {
        printf("Calling callbacks through js_ref\n");
        js_ref<IJsFunction<std::string(std::string)>> cb = tc::js::NewHeapCallback([](std::string str) noexcept {
            return tc::js::DeleteThisCallback("hello " + str);
        });
       _ASSERTEQUAL(cb->operator()("world"), "hello world");
    }
    return 0;
}
