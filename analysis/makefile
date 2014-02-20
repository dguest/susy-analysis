# makefile for SUSY ntuple skimmer
# Author: Dan Guest (dguest@cern.ch)
# Created: Wed Jul  4 15:30:40 CEST 2012

# --- set dirs
BIN          := bin
SRC          := src
INC          := include
DICT         := dict

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
vpath %.hh   $(INC) 
vpath %.h    $(INC) 

# --- load in root config
ROOTCFLAGS    := $(shell root-config --cflags)
ROOTLIBS      := -L$(shell root-config --libdir)
ROOTLIBS      += -lCore -lTree -lPhysics -lRIO
ROOTLDFLAGS   := $(shell root-config --ldflags)

# and py config
PY_CONFIG := python3.3-config

PY_FLAGS :=   $(shell $(PY_CONFIG) --includes)
PY_LIB   := -L$(shell $(PY_CONFIG) --prefix)/lib
PY_LIB   +=   $(shell $(PY_CONFIG) --libs)


# --- set compiler and flags (roll c options and include paths together)
CXX          := g++
CXXFLAGS     := -O2 -Wall -fPIC -I$(INC) -I$(ND_HIST_INC) -g -std=c++11
CXXFLAGS     += ${CXXFLAG_HACKS}
LIBS         := -L$(ND_HIST_LIB) -Wl,-rpath,$(ND_HIST_LIB) -lndhist
LDFLAGS      := -Wl,-no-undefined

CXXFLAGS     += -I$(HDF_PATH)/include
LIBS         += -L$(HDF_PATH)/lib -Wl,-rpath,$(HDF_PATH)/lib

# --- HDF5 needed for hist saving
LIBS         += -lhdf5_cpp -lhdf5 

# rootstuff 
CXXFLAGS     += $(ROOTCFLAGS)
LDFLAGS      += $(ROOTLDFLAGS)
LIBS         += $(ROOTLIBS)

# pystuff (roll the linking options and libraries together)
PY_LDFLAGS := $(LDFLAGS)
PY_LDFLAGS += $(PY_LIB)
PY_LDFLAGS += -shared

# ---- define objects
GEN_OBJ     := HistBuilder.o Jet.o SkimCounts.o ObjectFactory.o
GEN_OBJ     += JetTagRescaler.o JetBuffer.o BtagBuffer.o
GEN_OBJ     += BtagConfig.o RegionConfig.o Flavor.o
GEN_OBJ     += EventObjects.o
GEN_OBJ     += RegionEventFilter.o 

GEN_OBJ     += QualityEventSelection.o
GEN_OBJ     += SignalSelection.o CR1LSelection.o 
GEN_OBJ     += OSDFSelection.o OSSFSelection.o

GEN_OBJ     += OrderedJetTagFilter.o UnorderedJetTagFilter.o

GEN_OBJ     += NMinus1Histograms.o
GEN_OBJ     += Region2dKinematicHistograms.o
GEN_OBJ     += RegionJetEfficiencyHistograms.o
GEN_OBJ     += RegionBosonPtHistograms.o
GEN_OBJ     += RegionHistograms.o

GEN_OBJ     += Jet2DHists.o Jet1DHists.o TruthJetHists.o

GEN_OBJ     += common_functions.o enum_converters.o hdf_helpers.o
GEN_OBJ     += histogram_factory.o
GEN_OBJ     += EventScalefactors.o
EXE_OBJ     := $(GEN_OBJ) unit-test.o
PYLIB_OBJ   := $(GEN_OBJ)

STAND_ALONE_OBJ     := $(GEN_OBJ) stand-alone.o

PY_OBJ       := _hfw.o
PY_LIB       := ../python3/scharm/stack/_hfw.so


ALLOBJ       := $(GEN_OBJ) $(PY_OBJ) unit-test.o

ALLOUTPUT    := $(PY_LIB) unit-test stand-alone

# ---- all the building stuff

BUILD_SEQUENCE_DUMMY := initial-build-sequence
LINK_SEQUENCE_DUMMY := final-linking-sequence
NDHIST_DUMMY := build-ndhist
.PHONY: $(LINK_SEQUENCE) $(LINK_LIST) $(NDHIST_DUMMY)
all: $(BUILD_SEQUENCE_DUMMY)

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
$(PY_LIB): $(PYLIB_PATHS) $(ND_HIST_SO)
	@mkdir -p $(shell dirname $(PY_LIB))
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
	rm -fr $(BIN) $(ALLOUTPUT) $(DICT)
	@$(MAKE) -C $(ND_HIST_DIR) clean --no-print-directory

rmdep: 
	rm -f $(DEP)/*.d