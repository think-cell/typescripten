#include <emscripten/val.h>
#include <emscripten/bind.h>
#include <tc/assert_defs.h>
#include <typescripten/typescripten.h>

using tc::js::string;

int main() {
	string* o = new string("foo");
	delete o;
	// `o` is now dead, but it still keeps an emscripten::val's handle.
	// In my test it was 5. Embind clears corresponding entry of the objects table.
	// 
	// Then we call 'length' method. We start with storing 'length' string
	// in the objects table, taking the recently freed handle 5.
	// Next, the method is called on object #5 and returns 'length'.length, which is 6.
	//
	// Of course, such behavior is undefined and one should not rely on it,
	// but it's a great example of how things can get wrong.
	_ASSERTEQUAL(o->length(), 6);
}
