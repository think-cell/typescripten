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
ifdef PRECOMPILED
PRECOMPILED_OBJ=$(OBJDIR)/$(PRECOMPILED).pch
endif
PRE_JS+=$(wildcard $(BOOTSTRAP_PATH)/src/*.js)

all: $(TARGETS)

$(TARGETS) &: $(OBJS)
	test -n "$(EMCXX)"  # Expect $$EMCXX
	$(EMCXX) $(EMLDFLAGS) $(^:%="%") -o "$@"

$(OBJDIR)/%.o: %.cpp $(PRECOMPILED_OBJ) | $(OBJDIR)
	test -n "$(EMCXX)"  # Expect $$EMCXX
	$(EMCXX) $(EMCXXFLAGS) -MMD -MP -c "$<" $(PRECOMPILED_OBJ:%=-include-pch %) -o "$@"

ifdef PRECOMPILED
$(PRECOMPILED_OBJ): $(PRECOMPILED) | $(OBJDIR)
	test -n "$(EMCXX)"  # Expect $$EMCXX
	$(EMCXX) $(EMCXXFLAGS) -MMD -MP -c "$<" -x c++-header -o "$@"
endif

$(OBJDIR)/%.o: $(BOOTSTRAP_PATH)/src/%.cpp | $(OBJDIR)
	test -n "$(EMCXX)"  # Expect $$EMCXX
	$(EMCXX) $(EMCXXFLAGS) -MMD -MP -c "$<" -o "$@"

clean:
	rm -rf $(TARGETS) $(OBJDIR) *.wasm.map

$(OBJDIR):
	mkdir $@

-include $(OBJS:.o=.d) $(PRECOMPILED_OBJ:.pch=.d)
