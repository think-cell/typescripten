.PHONY: all

include ../../../build-flags.mak

all: demo-module-use.js

demo-module.d.h: ../../main.js demo-module.d.ts
	node ../../main.js demo-module.d.ts >demo-module.d.h

demo-module-use.js: demo-module-use.cpp demo-module.d.h ../../obj/precompiled.h.pch
	test -n "$(EMCXX)"  # Expect $$EMCXX
	$(EMCXX) $(EMCXXFLAGS) $(EMLDFLAGS) -o "$@" -include-pch ../../obj/precompiled.h.pch "$<"
