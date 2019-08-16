#include <emscripten/val.h>
#include "range.h"
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
    printf("length=%d; 2==%d\n", arr->length(), static_cast<int>(arr[1]));
    arr[1] = 15;
    printf("15==%d\n", static_cast<int>(arr[1]));

    tc::for_each(arr, [](int item) {
        printf("item[]=%d\n", item);
    });
    tc::for_each(tc::filter(tc::transform(arr, [](int x) { return x * x; }), [](int x) { return x >= 2; }), [](int item) {
        printf("filtered_transformed_item[]=%d\n", item);
    });
    return 0;
}
