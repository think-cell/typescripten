#include <emscripten/val.h>
#include "range_defines.h"
#include "js_bootstrap.h"
#include "js_callback.h"

using emscripten::val;

#define FOR_ALL_CALLBACKS(CreateCallback) \
    CreateCallback(TestSum, int, (int a, int b), { \
        _ASSERTEQUAL(a, 10); \
        _ASSERTEQUAL(b, 20); \
        return a + b; \
    })\
    CreateCallback(TestPrint, void, (std::string sMessage), { \
        _ASSERTEQUAL(sMessage, "hello"); \
    })\
    CreateCallback(TestPassAllArguments, void, (tc::js::pass_all_arguments_t, val emvalArgs, int a), { \
        _ASSERTEQUAL(a, 1); \
        _ASSERTEQUAL(emvalArgs[1].as<std::string>(), "message"); \
        _ASSERTEQUAL(emvalArgs["length"].as<int>(), 3); \
    }) \
    CreateCallback(TestPassThis, void, (tc::js::pass_this_t, val emvalThis, int a, std::string b, val c), { \
        _ASSERTEQUAL(emvalThis["secretField"].as<std::string>(), "secretValue"); \
        _ASSERTEQUAL(a, 1); \
        _ASSERTEQUAL(b, "message"); \
        _ASSERT(c.isNull()); \
    }) \
    CreateCallback(TestPassThisPassAllArguments, void, (tc::js::pass_this_t, val emvalThis, tc::js::pass_all_arguments_t, val emvalArgs, int a), { \
        _ASSERTEQUAL(emvalThis["secretField"].as<std::string>(), "secretValue"); \
        _ASSERTEQUAL(a, 1); \
        _ASSERTEQUAL(emvalArgs[1].as<std::string>(), "message"); \
        _ASSERTEQUAL(emvalArgs["length"].as<int>(), 3); \
    }) \
    CreateCallback(TestPassAllArgumentsAndReturn, std::string, (tc::js::pass_all_arguments_t, val emvalArgs, int a), { \
        _ASSERTEQUAL(a, 1); \
        _ASSERTEQUAL(emvalArgs[1].as<std::string>(), "message"); \
        _ASSERTEQUAL(emvalArgs["length"].as<int>(), 3); \
        return "returnValue"; \
    }) \
    CreateCallback(TestPassThisAndReturn, std::string, (tc::js::pass_this_t, val emvalThis, int a, std::string b, val c), { \
        _ASSERTEQUAL(emvalThis["secretField"].as<std::string>(), "secretValue"); \
        _ASSERTEQUAL(a, 1); \
        _ASSERTEQUAL(b, "message"); \
        _ASSERT(c.isNull()); \
        return "returnValue"; \
    }) \
    CreateCallback(TestPassThisPassAllArgumentsAndReturn, std::string, (tc::js::pass_this_t, val emvalThis, tc::js::pass_all_arguments_t, val emvalArgs, int a), { \
        _ASSERTEQUAL(emvalThis["secretField"].as<std::string>(), "secretValue"); \
        _ASSERTEQUAL(a, 1); \
        _ASSERTEQUAL(emvalArgs[1].as<std::string>(), "message"); \
        _ASSERTEQUAL(emvalArgs["length"].as<int>(), 3); \
        return "returnValue"; \
    })

#define RUN_TEST(Name, Callback) \
    { \
        printf(#Name " start\n"); \
        val::module_property(#Name)(Callback); \
        printf(#Name " end\n"); \
    }

int main() {
    {
        printf("===== CScopedCallback =====\n");
        #define CALL_SCOPED_CALLBACK(Name, ReturnType, Arguments, Body) \
            RUN_TEST(Name, tc::js::CScopedCallback([]Arguments noexcept -> ReturnType Body));
        FOR_ALL_CALLBACKS(CALL_SCOPED_CALLBACK)
        #undef CALL_SCOPED_CALLBACK
    }
    {
        printf("===== NewFiresOnceCallback =====\n");
        #define CALL_FIRES_ONCE_CALLBACK(Name, ReturnType, Arguments, Body) \
            RUN_TEST(Name, tc::js::NewFiresOnceCallback([]Arguments noexcept -> ReturnType Body));
        FOR_ALL_CALLBACKS(CALL_FIRES_ONCE_CALLBACK)
        #undef CALL_FIRES_ONCE_CALLBACK
    }
    {
        printf("===== TC_JS_MEMBER_FUNCTION =====\n");
        #define DEFINE_MEMBER(Name, ReturnType, Arguments, Body) TC_JS_MEMBER_FUNCTION(TestClass, m_emval##Name, ReturnType, Arguments) Body
        struct TestClass {
            FOR_ALL_CALLBACKS(DEFINE_MEMBER)
        } obj;
        #undef DEFINE_MEMBER
        #define CALL_MEMBER(Name, ReturnType, Arguments, Body) \
            RUN_TEST(Name, obj.m_emval##Name);
        FOR_ALL_CALLBACKS(CALL_MEMBER)
        #undef CALL_MEMBER
    }
    {
        printf("===== CHeapCallback =====\n");
        RUN_TEST(TestHeapCallback, tc::js::NewHeapCallback([expectedX = 2](int x) mutable noexcept {
            _ASSERTEQUAL(x, expectedX);
            expectedX--;
            if (x == 0)
                return tc::js::DeleteThisCallback();
            else
                return tc::js::KeepThisCallback();
        }));
        RUN_TEST(TestHeapCallbackWithReturn, tc::js::NewHeapCallback([expectedX = 2](int x) mutable noexcept {
            _ASSERTEQUAL(x, expectedX);
            expectedX--;
            if (x == 0)
                return tc::js::DeleteThisCallback(x + 10);
            else
                return tc::js::KeepThisCallback(x + 10);
        }));
        RUN_TEST(TestHeapCallbackPassAllArguments, tc::js::NewHeapCallback([expectedX = 2](tc::js::pass_all_arguments_t, val emvalArgs, int x) mutable noexcept {
            _ASSERTEQUAL(x, emvalArgs[0].as<int>());
            _ASSERTEQUAL(2, emvalArgs["length"].as<int>());
            _ASSERTEQUAL(x, expectedX);
            expectedX--;
            if (x == 0)
                return tc::js::DeleteThisCallback();
            else
                return tc::js::KeepThisCallback();
        }));
    }
    return 0;
}
