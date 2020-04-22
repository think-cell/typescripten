#include "../../precompiled.h"
#include "lib.dom.d.h"

int main() {
	tc::js::document()->title(tc::js_types::js_string("Hello World from C++!"));
	printf("Success!\n");
}
