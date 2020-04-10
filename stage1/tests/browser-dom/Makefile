.PHONY: all
TARGET=main.html
LOG=lib-dom-transform.log

include ../../../build-flags.mak

PRE_JS=

all: $(TARGET)

lib.dom.d.h: lib.dom.d.ts ../../main.js
	node ../../main.js $< >$@ 2>$(LOG)

$(TARGET): main.cpp another-ts.cpp lib.dom.d.h ../../obj/precompiled.h.pch
	test -n "$(EMCXX)"  # Expect $$EMCXX
	$(EMCXX) $(EMCXXFLAGS) $(EMLDFLAGS) -o "$@" -include-pch ../../obj/precompiled.h.pch main.cpp another-ts.cpp

clean:
	rm -rf MyLib.d.h $(TARGET) MyLib.js *.wasm *.wasm.map $(LOG)
