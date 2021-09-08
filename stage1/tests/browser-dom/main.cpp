#include "../../precompiled.h"

#pragma push_macro("assert")
#undef assert
#include "lib.dom.d.h"
#pragma pop_macro("assert")

int main() {
	tc::js::document()->title(tc::js::string("Hello World from C++!"));
	static struct Clicker {
		int number = 1;
		TC_JS_MEMBER_FUNCTION(Clicker, m_jfuncOnClick, tc::js::any, (tc::js::MouseEvent ev)) {
			printf("Got click, number is %d; x=%f, y=%f\n", number, ev->x(), ev->y());
			tc::js::document()->title(tc::js::string(tc::concat(
				"Hello World from C++! number = ", tc::as_dec(number)
			)));
			number++;
			return tc::js::undefined{};
		}
	} clicker;
	tc::js::onclick(clicker.m_jfuncOnClick);

	// tc::js::document()->addEventListener(tc::js::string("click"), clicker.m_jfuncOnClick);

	printf("Success!\n");
}
