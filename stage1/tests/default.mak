.PHONY: all

TARGET?=main.js
PRE_JS=main-pre.js

include ../../../build-flags.mak

all: $(TARGET)

ifndef NO_TYPESCRIPT_COMPILATION
MyLib.js MyLib.d.ts: MyLib.ts
	../../node_modules/typescript/bin/tsc --strict --declaration "$<"
endif

MyLib.d.h: ../../main.js MyLib.d.ts
	node ../../main.js MyLib.d.ts >MyLib.d.h

$(TARGET): main.cpp another-ts.cpp main-pre.js MyLib.d.h ../../obj/precompiled.h.pch
	test -n "$(EMCXX)"  # Expect $$EMCXX
	$(EMCXX) $(EMCXXFLAGS) $(EMLDFLAGS) -o "$@" -include-pch ../../obj/precompiled.h.pch main.cpp another-ts.cpp

clean:
	rm -rf MyLib.d.h $(TARGET) MyLib.js *.wasm *.wasm.map
ifndef NO_TYPESCRIPT_COMPILATION
	rm -rf MyLib.d.ts
endif
