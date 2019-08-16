#include <emscripten/val.h>
#include <optional>
#include <vector>
#include "range.h"
#include "range_defines.h"
#include "js_bootstrap.h"
#include "js_callback.h"

using emscripten::val;
using tc::js::js_ref;
using tc::js::IAny;
using tc::js::globals::Array;
using tc::js::globals::String;
using tc::js::globals::console;

int main() {
    {
        js_ref<String> message(val("Hello World"));
        _ASSERTEQUAL(message->length(), 11);
        _ASSERTEQUAL(message->operator std::string(), "Hello World");
        console()->log(message);
    }

    js_ref<Array<int>> arr(val::array());
    arr->push(1);
    arr->push(2);
    arr->push(3);
    console()->log(arr);
    _ASSERTEQUAL(arr->length(), 3);
    _ASSERTEQUAL(arr[1], 2);
    arr[1] = 15;
    _ASSERTEQUAL(arr[1], 15);

    {
        std::vector<int> result;
        tc::for_each(arr, [&](int item) { result.push_back(item); });
        _ASSERTEQUAL(result, (std::vector{1, 15, 3}));
    }

    {
        std::vector<int> result;
        tc::for_each(
            tc::filter(
                tc::transform(arr,
                    [](int x) { return x * x; }
                ),
                [](int x) { return x >= 2; }
            ),
            [&](int item) { result.push_back(item); }
        );
        _ASSERTEQUAL(result, (std::vector{225, 9}));
    }

    {
        // Test optional
        using OptionalAny = std::optional<tc::js::js_ref<tc::js::IAny>>;
        static_assert(tc::js::IsJsRefOptional<OptionalAny>::value);
        static_assert(
            std::is_same<
                typename emscripten::internal::BindingType<OptionalAny>::WireType,
                typename emscripten::internal::BindingType<emscripten::val>::WireType
            >::value);

        {
            emscripten::val emval{OptionalAny()};
            _ASSERT(!emval.template as<OptionalAny>());
            _ASSERT(emval.isUndefined());
        }
        {
            emscripten::val emvalOrigin = emscripten::val::object();
            emscripten::val emval{OptionalAny(emvalOrigin)};
            auto oanyParsed = emval.template as<OptionalAny>();
            _ASSERT(oanyParsed);
            _ASSERT(oanyParsed->get().strictlyEquals(emvalOrigin));
            _ASSERT(!emval.isUndefined());
            _ASSERT(emval.strictlyEquals(emvalOrigin));
        }
    }
    return 0;
}
