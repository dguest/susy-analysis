# makefile for SUSY ntuple skimmer
# Author: Dan Guest (dguest@cern.ch)
# Created: Wed Jul  4 15:30:40 CEST 2012

# --- set dirs
BIN          := bin
SRC          := src
INC          := src
DICT         := dict

# --- external dirs 
-include local.mk		# should define ROOTCORE_ROOT

ifndef ROOTCORE_108_ROOT
$(error "couldn't find ROOTCORE_108_ROOT, is it defined in local.mk?")
endif
ROOTCORE_ROOT := $(ROOTCORE_108_ROOT)
SUSY_INC      := $(shell ./map_libs.sh -i $(ROOTCORE_ROOT))

#  set search path
vpath %.o    $(BIN)
vpath %.cxx  $(SRC) 
vpath %.hh   $(INC) $(SUSY_INC)
vpath %.h    $(INC) $(SUSY_INC)
vpath %Dict.h $(DICT)
vpath %Dict.cxx $(DICT)

# --- load in root config
ROOTCFLAGS    := $(shell root-config --cflags)
ROOTLIBS      := $(shell root-config --libs)
ROOTLDFLAGS   := $(shell root-config --ldflags)
ROOTLIBS      += -lTreePlayer 	#don't know why this isn't loaded by default

# and py config
PY_CONFIG := python2.7-config

PY_FLAGS :=   $(shell $(PY_CONFIG) --includes)
PY_LIB   := -L$(shell $(PY_CONFIG) --prefix)/lib
PY_LIB   +=   $(shell $(PY_CONFIG) --libs)


# --- set compiler and flags (roll c options and include paths together)
CXX          := g++
CXXFLAGS     := -O2 -Wall -fPIC -I$(INC) $(SUSY_INC:%=-I%) -g
LDFLAGS      := -Wl,-no-undefined
LIBS         := $(shell ./map_libs.sh -l $(ROOTCORE_ROOT))

# rootstuff 
CXXFLAGS     += $(ROOTCFLAGS)
LDFLAGS      += $(ROOTLDFLAGS)
LIBS         += $(ROOTLIBS)

# pystuff (roll the linking options and libraries together)
PY_LDFLAGS := $(LDFLAGS)
PY_LDFLAGS += $(PY_LIB)
PY_LDFLAGS += -shared

# ---- define objects
TOBJ        := SusyBuffer.o
T_DICTS     := $(TOBJ:.o=Dict.o)
GEN_OBJ     := cutflow.o OutTree.o
EXE_OBJ     := $(GEN_OBJ) $(T_DICTS) $(TOBJ)
PYLIB_OBJ   := $(GEN_OBJ) $(T_DICTS) $(TOBJ)

PY_OBJ       := _cutflow.o

ALLOBJ       := $(GEN_OBJ) $(PY_OBJ)

ALLOUTPUT    := cutflow python/susy/_cutflow.so

all: $(ALLOUTPUT) 

cutflow: $(EXE_OBJ:%=$(BIN)/%)
	@echo "linking $^ --> $@"
	@$(CXX) -o $@ $^ $(LIBS) $(LDFLAGS)

python/susy/_cutflow.so: $(PYLIB_OBJ:%=$(BIN)/%) $(PY_OBJ:%=$(BIN)/%)
	@mkdir -p python/susy
	@echo "linking $^ --> $@"
	@$(CXX) -o $@ $^ $(LIBS) $(PY_LDFLAGS)

# --------------------------------------------------

# python object compile
$(BIN)/_%.o: _%.cxx 
	@echo compiling python object $@
	@mkdir -p $(BIN)
	@$(CXX) -c $(CXXFLAGS) $(PY_FLAGS) $< -o $@ 

# root dictionary generation 
$(DICT)/%Dict.cxx: %.h LinkDef.hh
	@echo making dict $@
	@mkdir -p $(DICT)
	@rm -f $(DICT)/$*Dict.h $(DICT)/$*Dict.cxx 
	@rootcint $@ -c $(INC)/$*.h	
	@sed -i 's,#include "$(INC)/\(.*\)",#include "\1",g' $(DICT)/$*Dict.h

$(BIN)/%Dict.o: $(DICT)/%Dict.cxx 
	@mkdir -p $(BIN)
	@echo compiling dict $@
	@$(CC) $(CXXFLAGS) $(ROOTCFLAGS) -c $< -o $@

# compile rule
$(BIN)/%.o: %.cxx
	@echo compiling $<
	@mkdir -p $(BIN)
	@$(CXX) -c $(CXXFLAGS) $< -o $@

# use auto dependency generation
DEP = $(BIN)

ifneq ($(MAKECMDGOALS),clean)
ifneq ($(MAKECMDGOALS),rmdep)
include  $(ALLOBJ:%.o=$(DEP)/%.d)
endif
endif

$(DEP)/%.d: %.cxx
	@echo making dependencies for $<
	@mkdir -p $(DEP)
	@$(CXX) -MM $(CXXFLAGS) $< -o $@
	@sed -i 's,$*\.o,$(BIN)/& $@ ,g' $@

# clean
.PHONY : clean rmdep
CLEANLIST     = *~ *.o *.o~ *.d core 
clean:
	rm -fr $(CLEANLIST) $(CLEANLIST:%=$(BIN)/%) $(CLEANLIST:%=$(DEP)/%)
	rm -fr $(BIN) $(ALLOUTPUT) $(DICT)

rmdep: 
	rm -f $(DEP)/*.d