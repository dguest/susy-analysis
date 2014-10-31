# makefile for SUSY ntuple skimmer
# Author: Dan Guest (dguest@cern.ch)
# Created: Wed Jul  4 15:30:40 CEST 2012

# --- set dirs
BIN          := bin
SRC          := src
INC          := include
DICT         := dict

SELECTION_SUBDIR := selection
SELECTION_SRC := $(SRC)/$(SELECTION_SUBDIR)
SELECTION_INC := $(INC)/$(SELECTION_SUBDIR)

# --- HACKS ----
CXXFLAG_HACKS := -Wno-literal-suffix #hdf5 header sets this off

# --- hdf and ndhist
HDF_INFO := $(shell h5c++ -showconfig | grep 'Installation point:')
HDF_PATH := $(strip $(shell echo ${HDF_INFO} | cut -d ':' -f 2 ))
ifndef HDF_PATH
$(error "couldn't find HDF, quitting")
endif

ND_HIST_DIR      := $(CURDIR)/ndhist
ND_HIST_INC      := $(ND_HIST_DIR)/include
ND_HIST_LIB      := $(ND_HIST_DIR)/lib
ND_HIST_SO       := $(ND_HIST_LIB)/libndhist.so

#  set search path
vpath %.o    $(BIN)
vpath %.cxx  $(SRC)
vpath %Selection.cxx $(SELECTION_SRC)

# --- load in root config
ROOTCFLAGS    := $(shell root-config --cflags)
ROOTLIBS      := -L$(shell root-config --libdir)
ROOTLIBS      += -lCore -lTree -lPhysics -lRIO
ROOTLDFLAGS   := $(shell root-config --ldflags)

# and py config
PY_CONFIG := python3.3-config

PY_FLAGS :=   $(shell $(PY_CONFIG) --includes)
PY_LIBS   := -L$(shell $(PY_CONFIG) --prefix)/lib
PY_LIBS   +=   $(shell $(PY_CONFIG) --libs)


# --- set compiler and flags (roll c options and include paths together)
CXX          := g++
CXXFLAGS     := -O2 -Wall -fPIC -I$(INC) -I$(ND_HIST_INC) -g -std=c++11
CXXFLAGS     += ${CXXFLAG_HACKS}
LIBS         := -L$(ND_HIST_LIB) -Wl,-rpath,$(ND_HIST_LIB) -lndhist
LDFLAGS      := -Wl,-no-undefined

CXXFLAGS     += -I$(HDF_PATH)/include -I$(SELECTION_INC)
LIBS         += -L$(HDF_PATH)/lib -Wl,-rpath,$(HDF_PATH)/lib

# --- HDF5 needed for hist saving
LIBS         += -lhdf5_cpp -lhdf5

# rootstuff
CXXFLAGS     += $(ROOTCFLAGS)
LDFLAGS      += $(ROOTLDFLAGS)
LIBS         += $(ROOTLIBS)

# pystuff (roll the linking options and libraries together)
PY_LDFLAGS := $(LDFLAGS)
PY_LDFLAGS += $(PY_LIBS)
PY_LDFLAGS += -shared

# ---- define objects
# core objects
GEN_OBJ     := HistBuilder.o Jet.o SkimCounts.o ObjectFactory.o
GEN_OBJ     += JetBuffer.o BtagBuffer.o
GEN_OBJ     += BtagConfig.o RegionConfig.o Flavor.o
GEN_OBJ     += EventObjects.o
GEN_OBJ     += EventScalefactors.o TTBarReweighting.o
GEN_OBJ     += NMinusHist.o
GEN_OBJ     += Lepton.o LeptonBuffer.o

# selection classes
GEN_OBJ     += QualityEventSelection.o MetSelection.o
GEN_OBJ     += SignalSelection.o CR1LSelection.o
GEN_OBJ     += CR1ESelection.o CR1MSelection.o
GEN_OBJ     += OSDFSelection.o OSSFSelection.o
GEN_OBJ     += CRZSelection.o
GEN_OBJ     += VRMCTSelection.o VRMCCSelection.o

GEN_OBJ     += Window.o
GEN_OBJ     += NMinus1Histograms.o
GEN_OBJ     += EventList.o
GEN_OBJ     += nminus_tools.o

# more common functions
GEN_OBJ     += common_functions.o hdf_helpers.o
GEN_OBJ     += trigger_logic.o region_logic.o
GEN_OBJ     += StackerExceptions.o
GEN_OBJ     += histogram_factory.o
EXE_OBJ     := $(GEN_OBJ) unit-test.o
PYLIB_OBJ   := $(GEN_OBJ)

STAND_ALONE_OBJ     := $(GEN_OBJ) stand-alone.o

PY_OBJ       := _hfw.o
PY_SO        := ../python3/scharm/stack/_hfw.so
PY_SO_PROD   := ../python3/scharm/stack/_hfw_prod.so


ALLOBJ       := $(GEN_OBJ) $(PY_OBJ) unit-test.o

ALLOUTPUT    := $(PY_SO) unit-test stand-alone

# ---- all the building stuff

BUILD_SEQUENCE_DUMMY := initial-build-sequence
LINK_SEQUENCE_DUMMY := final-linking-sequence
NDHIST_DUMMY := build-ndhist
.PHONY: $(LINK_SEQUENCE_DUMMY) $(LINK_SEQUENCE_DUMMY) $(NDHIST_DUMMY)

all: $(BUILD_SEQUENCE_DUMMY)
prod: $(BUILD_SEQUENCE_DUMMY)
	@cp $(PY_SO) $(PY_SO_PROD)
	@echo copied library to production

ALL_OBJ_PATHS := $(ALL_OBJ:%=$(BIN)/%)

# we call the dummy first, which builds the dependencies.
# _after_ these have been built we call the linking rule.
$(BUILD_SEQUENCE_DUMMY): $(NDHIST_DUMMY) $(ALL_OBJ_PATHS)
	@$(MAKE) $(LINK_SEQUENCE_DUMMY) --no-print-directory

$(LINK_SEQUENCE_DUMMY): $(ALLOUTPUT)

$(NDHIST_DUMMY):
	@$(MAKE) -C $(ND_HIST_DIR)

EXE_PATHS := $(EXE_OBJ:%=$(BIN)/%)
unit-test: $(EXE_PATHS) $(ND_HIST_SO)
	@echo "linking $^ --> $@"
	@$(CXX) -o $@ $(EXE_PATHS) $(LIBS) $(LDFLAGS)

STAND_ALONE_PATHS := $(STAND_ALONE_OBJ:%=$(BIN)/%)
stand-alone: $(STAND_ALONE_PATHS) $(ND_HIST_SO)
	@echo "linking $^ --> $@"
	@$(CXX) -o $@ $(STAND_ALONE_PATHS) $(LIBS) $(LDFLAGS)

PYLIB_PATHS := $(PYLIB_OBJ:%=$(BIN)/%) $(PY_OBJ:%=$(BIN)/%)
$(PY_SO): $(PYLIB_PATHS) $(ND_HIST_SO)
	@mkdir -p $(shell dirname $(PY_SO))
	@echo "linking $^ --> $@"
	@$(CXX) -o $@ $(PYLIB_PATHS) $(LIBS) $(PY_LDFLAGS)

# --------------------------------------------------

# python object compile
$(BIN)/_%.o: _%.cxx
	@echo compiling python object $@
	@mkdir -p $(BIN)
	@$(CXX) -c $(CXXFLAGS) $(PY_FLAGS) $< -o $@

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

DEPTARGSTR = -MT $(BIN)/$*.o -MT $(DEP)/$*.d
$(DEP)/%.d: %.cxx
	@echo making dependencies for $<
	@mkdir -p $(DEP)
	@$(CXX) -MM -MP $(DEPTARGSTR) $(CXXFLAGS) $(PY_FLAGS) $< -o $@

# clean
.PHONY : clean rmdep
CLEANLIST     = *~ *.o *.o~ *.d core
clean:
	rm -fr $(BIN) $(ALLOUTPUT) $(DICT) $(PY_SO_PROD)
	@$(MAKE) -C $(ND_HIST_DIR) clean --no-print-directory

rmdep:
	rm -f $(DEP)/*.d