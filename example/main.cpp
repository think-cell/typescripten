#include <typescripten/typescripten.h>

#include <lib.dom.d.h>

namespace js = tc::js; // top-level JS namespace 

int main() {
	js::document()->title(js::string("Hello World from C++!"));

	static struct Clicker {
		js::HTMLElement elem;
		int m_nClicks = 0;

		Clicker() noexcept 
			// var elem = document.createElement("p")
			: elem(js::document()->createElement(js::string("p"))) 
		{
			// elem.innerText = "Hello World"
			elem->innerText(js::string("Click Me"));

			// elem.style.fontSize = "20vh"
			elem->style()->fontSize(js::string("20vh"));
			
			// document.body.appendChild(elem)
			js::document()->body()->appendChild(elem);
		}

		TC_JS_MEMBER_FUNCTION(Clicker, m_jfuncOnClick, tc::js::any, (tc::js::MouseEvent)) {
			elem->innerText(js::string(tc::concat(
				"Hello World from C++ #", tc::as_dec(m_nClicks)
			)));
			++m_nClicks;
			return tc::js::undefined{};
		}
	} clicker;
	tc::js::document()->addEventListener("click"_s, clicker.m_jfuncOnClick);
}
