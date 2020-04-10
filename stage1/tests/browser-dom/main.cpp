#include "../../precompiled.h"
#include "lib.dom.d.h"

int main() {
	tc::js::globals::document()->write("Hello World!");
	printf("Success!\n");
}
