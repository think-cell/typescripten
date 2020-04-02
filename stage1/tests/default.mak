.PHONY: all

include ../../../build-flags.mak

ifdef COMPILATION_ONLY
all: main.js
else
all: main.js my-lib.js
	node $<

my-lib.js: my-lib.ts
	tsc --strict --declaration "$<"
endif

my-lib.d.h: ../../main.js my-lib.d.ts
	node ../../main.js my-lib.d.ts >my-lib.d.h

main.js: main.cpp main-pre.js my-lib.d.h ../../obj/precompiled.h.pch
	test -n "$(EMCXX)"  # Expect $$EMCXX
	$(EMCXX) $(EMCXXFLAGS) $(EMLDFLAGS) --pre-js main-pre.js -o "$@" -include-pch ../../obj/precompiled.h.pch "$<"

clean:
	rm -rf my-lib.d.h *.js *.wasm *.wasm.map
