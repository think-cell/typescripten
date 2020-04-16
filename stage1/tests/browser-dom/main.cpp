#include "../../precompiled.h"
#include "lib.dom.d.h"

int main() {
	tc::js::globals::document()->title(tc::js::js_string("Hello World from C++!"));
	printf("Success!\n");
}
