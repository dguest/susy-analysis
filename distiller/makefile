# makefile for SUSY ntuple skimmer
# Author: Dan Guest (dguest@cern.ch)
# Created: Wed Jul  4 15:30:40 CEST 2012

# --- set dirs
BIN          := bin
SRC          := src
INC          := include
DICT         := dict

# --- external dirs 
-include local.mk		# should define ROOTCORE_ROOT

ifndef ROOTCORE_312_ROOT
$(error "couldn't find ROOTCORE_312_ROOT, is it defined in local.mk?")
endif
ROOTCORE_ROOT := $(ROOTCORE_312_ROOT)
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
ROOTLIBS      += -lTreePlayer   #don't know why this isn't loaded by default
ROOTLIBS      += -lTMVA         #don't know why this isn't loaded by default
ROOTLIBS      += -lXMLParser    #don't know why this isn't loaded by default
ROOTLIBS      += -lEG           #for TParticle

# and py config
PY_CONFIG := python2.7-config

PY_FLAGS :=   $(shell $(PY_CONFIG) --includes)
PY_LIB   := -L$(shell $(PY_CONFIG) --prefix)/lib
PY_LIB   +=   $(shell $(PY_CONFIG) --libs)


# --- set compiler and flags (roll c options and include paths together)
CXX          := g++
CXXFLAGS     := -O2 -Wall -fPIC -I$(INC) $(SUSY_INC:%=-I%) -g -std=c++11
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

# dependency flags
DEP        := $(BIN)
DEPINCLUDE := -I$(INC) $(SUSY_INC:%=-I%) -I$(shell root-config --incdir)
DEPFLAGS    = -M -MP -MT $(BIN)/$*.o -MT $(DEP)/$*.d $(DEPINCLUDE) $(PY_FLAGS)

# ---- define objects
TOBJ        := SusyBuffer.o
T_DICTS     := $(TOBJ:.o=Dict.o)
GEN_OBJ     := distill_tools.o Jets.o Leptons.o Met.o
GEN_OBJ     += OutTree.o OutputFilter.o BitmapCutflow.o 
GEN_OBJ     += object_selection.o
GEN_OBJ     += BosonTruthFilter.o TruthMetFilter.o
GEN_OBJ     += SmartChain.o CollectionTreeReport.o 
GEN_OBJ     += EventPreselector.o StopDistiller.o CheckSize.o
GEN_OBJ     += BtagCalibration.o Grl.o RunInfo.o
GEN_OBJ     += unit_tests.o
EXE_OBJ     := $(GEN_OBJ) $(T_DICTS) $(TOBJ) 
PYLIB_OBJ   := $(GEN_OBJ) $(T_DICTS) $(TOBJ)

PY_OBJ       := _distiller.o
BITDEFS      := ../python/stop/bits.yml

ALLOBJ       := $(GEN_OBJ) $(PY_OBJ)

PY_OUT_PATH  := ../python/stop/distiller/$(PY_OBJ:.o=.so)
ALLOUTPUT    := $(PY_OUT_PATH) unit-test
ALLOUTPUT    += $(BITDEFS)

all: $(ALLOUTPUT) 

cutflow: $(EXE_OBJ:%=$(BIN)/%)
	@echo "linking $^ --> $@"
	@$(CXX) -o $@ $^ $(LIBS) $(LDFLAGS)
unit-test: $(EXE_OBJ:%=$(BIN)/%)
	@echo "linking $^ --> $@"
	@$(CXX) -o $@ $^ $(LIBS) $(LDFLAGS)

$(PY_OUT_PATH): $(PYLIB_OBJ:%=$(BIN)/%) $(PY_OBJ:%=$(BIN)/%)
	@mkdir -p python/susy
	@echo "linking $^ --> $@"
	@$(CXX) -o $@ $^ $(LIBS) $(PY_LDFLAGS)
$(BITDEFS): EventBits.hh
	@echo 'rewriting $@'
	@./get_event_bits.sh >| $@

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
	@$(CXX) $(CXXFLAGS) $(ROOTCFLAGS) -c $< -o $@

# compile rule
$(DEP)/%.o: %.cxx
	@echo compiling $<
	@mkdir -p $(DEP)
	@$(CXX) -c $(CXXFLAGS) $< -o $@

# use auto dependency generation

ifneq ($(MAKECMDGOALS),clean)
ifneq ($(MAKECMDGOALS),rmdep)
include  $(ALLOBJ:%.o=$(DEP)/%.d)
endif
endif

$(BIN)/%.d: %.cxx
	@echo making dependencies for $<
	@mkdir -p $(BIN)
	@$(CXX) $(DEPFLAGS) $< -o $@ 

# clean
.PHONY : clean rmdep
CLEANLIST     = *~ *.o *.o~ *.d core 
clean:
	rm -fr $(CLEANLIST) $(CLEANLIST:%=$(BIN)/%) $(CLEANLIST:%=$(DEP)/%)
	rm -fr $(BIN) $(ALLOUTPUT) $(DICT)

rmdep: 
	rm -f $(DEP)/*.d