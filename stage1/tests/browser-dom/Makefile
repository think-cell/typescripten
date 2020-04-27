.PHONY: all
TARGET=main.html
LOG=lib-dom-transform.log

include ../../../build-flags.mak

BOOTSTRAP_SRCS=$(wildcard $(BOOTSTRAP_PATH)/src/*.cpp)

PRE_JS=$(wildcard $(BOOTSTRAP_PATH)/src/*.js)

all: $(TARGET)

lib.dom.d.ts: ../../node_modules/typescript/lib/lib.dom.d.ts
	cp $< $@

lib.dom.d.h: lib.dom.d.ts ../../main.js
	node ../../main.js $< >$@ 2>$(LOG)

$(TARGET): main.cpp another-ts.cpp $(BOOTSTRAP_SRCS) lib.dom.d.h ../../obj/precompiled.h.pch
	test -n "$(EMCXX)"  # Expect $$EMCXX
	$(EMCXX) $(EMCXXFLAGS) $(EMLDFLAGS) -o "$@" -include-pch ../../obj/precompiled.h.pch main.cpp another-ts.cpp $(BOOTSTRAP_SRCS:%="%")

clean:
	rm -rf MyLib.d.h $(TARGET) MyLib.js *.wasm *.wasm.map $(LOG)
