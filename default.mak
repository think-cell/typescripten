.PHONY: all clean

ifndef TARGET
$(error "$$TARGET variable should be specified")
endif
ifndef SRCS
$(error "$$SRCS variable should be specified")
endif
ifndef PRE_JS
$(error "$$PRE_JS variable should be specified")
endif

TARGETS=$(TARGET) $(TARGET:.js=.wasm) $(TARGET:.js=.wasm.map)
OBJDIR=obj
SRCS+=$(wildcard $(BOOTSTRAP_PATH)/src/*.cpp)
OBJS=$(patsubst %.cpp,$(OBJDIR)/%.o,$(notdir $(SRCS)))
PRE_JS+=$(wildcard $(BOOTSTRAP_PATH)/src/*.js)

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

EMLDFLAGS+=--bind
EMLDFLAGS+=$(PRE_JS:%=--pre-js "%")

all: $(TARGETS)

$(TARGETS) &: $(OBJS)
	test -n "$(EMCXX)"  # Expect $$EMCXX
	$(EMCXX) $(EMLDFLAGS) $(^:%="%") -o "$@"

$(OBJDIR)/%.o: %.cpp | $(OBJDIR)
	test -n "$(EMCXX)"  # Expect $$EMCXX
	$(EMCXX) $(EMCXXFLAGS) -MMD -MP -c "$<" -o "$@"

$(OBJDIR)/%.o: $(BOOTSTRAP_PATH)/src/%.cpp | $(OBJDIR)
	test -n "$(EMCXX)"  # Expect $$EMCXX
	$(EMCXX) $(EMCXXFLAGS) -MMD -MP -c "$<" -o "$@"

clean:
	rm -rf $(TARGETS) $(OBJDIR) *.wasm.map

$(OBJDIR):
	mkdir $@

-include $(OBJS:.o=.d)
