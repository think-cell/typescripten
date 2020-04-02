.PHONY: all

include ../../../build-flags.mak

all: main.js

my-lib.d.h: ../../main.js my-lib.d.ts
	node ../../main.js my-lib.d.ts >my-lib.d.h

main.js: main.cpp my-lib.d.h ../../obj/precompiled.h.pch
	test -n "$(EMCXX)"  # Expect $$EMCXX
	$(EMCXX) $(EMCXXFLAGS) $(EMLDFLAGS) -o "$@" -include-pch ../../obj/precompiled.h.pch "$<"

clean:
	rm -rf my-lib.d.h *.js *.wasm *.wasm.map
