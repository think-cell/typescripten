#include "../../precompiled.h"

#include <lib.dom.d.h>

int main() {
	tc::js::document()->title(tc::js::string("Hello World from C++!"));
	static struct Clicker {
		int i = 1;
		TC_JS_MEMBER_FUNCTION(Clicker, m_jfuncOnClick, tc::js::any, (tc::js::MouseEvent ev)) {
		std::printf("Got click, number is %d; x=%f, y=%f\n", i, ev->x(), ev->y());
			tc::js::document()->title(tc::js::string(tc::concat(
				"Hello World from C++! number = ", tc::as_dec(i)
			)));
			i++;
			return tc::js::undefined{};
		}
	} clicker;
	tc::js::document()->addEventListener("click"_s, clicker.m_jfuncOnClick);

	std::printf("Success!\n");
}
