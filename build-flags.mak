ifndef PRE_JS
$(error "$$PRE_JS variable should be specified")
endif

EMCXXFLAGS?=$(CXXFLAGS)
EMLDFLAGS?=$(LDFLAGS)

EMCXXFLAGS+=-std=c++17
ifdef BOOTSTRAP_PATH
EMCXXFLAGS+=-I "$(BOOTSTRAP_PATH)/include"
endif
ifdef THINK_CELL_PUBLIC_PATH
EMCXXFLAGS+=-isystem "$(THINK_CELL_PUBLIC_PATH)"
endif
ifdef BOOST_PATH
EMCXXFLAGS+=-isystem "$(BOOST_PATH)"
endif

PRE_JS+=$(wildcard $(BOOTSTRAP_PATH)/src/*.js)

EMLDFLAGS+=--bind
EMLDFLAGS+=$(PRE_JS:%=--pre-js "%")
