#include "../../precompiled.h"
#include "lib.dom.d.h"

int main() {
	tc::js::document()->title(tc::js_types::js_string("Hello World from C++!"));
	static struct Clicker {
		int number = 1;
		TC_JS_MEMBER_FUNCTION(Clicker, m_jfuncOnClick, tc::js_types::js_unknown, (tc::js::MouseEvent ev)) {
			printf("Got click, number is %d; x=%f, y=%f\n", number, ev->x(), ev->y());
			tc::js::document()->title(tc::js_types::js_string(tc::concat(
				"Hello World from C++! number = ", tc::as_dec(number)
			)));
			number++;
			return tc::js_types::js_undefined{};
		}
	} clicker;
	tc::js::onclick(clicker.m_jfuncOnClick);
	printf("Success!\n");
}
