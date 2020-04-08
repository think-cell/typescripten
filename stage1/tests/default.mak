.PHONY: all

PRE_JS=main-pre.js

include ../../../build-flags.mak

ifdef COMPILATION_ONLY
all: main.js
else
all: main.js MyLib.js
	node $<

MyLib.js MyLib.d.ts: MyLib.ts
	../../node_modules/typescript/bin/tsc --strict --declaration "$<"
endif

MyLib.d.h: ../../main.js MyLib.d.ts
	node ../../main.js MyLib.d.ts >MyLib.d.h

main.js: main.cpp another-ts.cpp main-pre.js MyLib.d.h ../../obj/precompiled.h.pch
	test -n "$(EMCXX)"  # Expect $$EMCXX
	$(EMCXX) $(EMCXXFLAGS) $(EMLDFLAGS) -o "$@" -include-pch ../../obj/precompiled.h.pch main.cpp another-ts.cpp

clean:
	rm -rf MyLib.d.h *.js *.wasm *.wasm.map
