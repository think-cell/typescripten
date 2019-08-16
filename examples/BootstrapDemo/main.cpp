#include <emscripten/val.h>
#include "js_bootstrap.h"
#include "js_callback.h"

using emscripten::val;
using tc::js::js_ref;
using tc::js::IAny;
using tc::js::globals::Array;
using tc::js::globals::console;

int main() {
    console()->log(js_ref<IAny>(val("Hello World!")));

    js_ref<Array<int>> arr(val::array());
    arr->push(1);
    arr->push(2);
    arr->push(3);
    console()->log(arr);
    printf("length=%d; 2==%d\n", arr->length(), arr[1]);

    arr.get().call<void>("forEach", tc::js::CScopedCallback([](int item, val, val) noexcept {
        printf("item[]=%d\n", item);
    }));
    return 0;
}
