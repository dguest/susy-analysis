# makefile for SUSY ntuple skimmer
# Author: Dan Guest (dguest@cern.ch)
# Created: Wed Jul  4 15:30:40 CEST 2012

# --- set dirs
BIN          := bin
SRC          := src
INC          := src

# --- external dirs 
SUSY_INC      := $(shell ./map_libs.sh -i $(ROOTCORE_ROOT))

#  set search path
vpath %.o    $(BIN)
vpath %.cxx  $(SRC) 
vpath %.hh   $(INC) $(SUSY_INC)
vpath %.h    $(INC) $(SUSY_INC)

# --- load in root config
ROOTCFLAGS    := $(shell root-config --cflags)
ROOTLIBS      := $(shell root-config --libs)
ROOTLDFLAGS   := $(shell root-config --ldflags)
ROOTLIBS      += -lTreePlayer 	#don't know why this isn't loaded by default

# # and py config
# PY_CONFIG := python2.7-config

# PY_FLAGS :=   $(shell $(PY_CONFIG) --includes)
# PY_LIB   := -L$(shell $(PY_CONFIG) --prefix)/lib
# PY_LIB   +=   $(shell $(PY_CONFIG) --libs)


# --- set compiler and flags (roll c options and include paths together)
CXX          := g++
CXXFLAGS     := -O2 -Wall -fPIC -I$(INC) $(SUSY_INC:%=-I%) -g
LDFLAGS      := -Wl,-no-undefined
LIBS         := $(shell ./map_libs.sh -l $(ROOTCORE_ROOT))

# rootstuff 
CXXFLAGS     += $(ROOTCFLAGS)
LDFLAGS      += $(ROOTLDFLAGS)
LIBS         += $(ROOTLIBS)

# # pystuff (roll the linking options and libraries together)
# PY_LDFLAGS := $(LDFLAGS)
# PY_LDFLAGS += $(PY_LIB)
# PY_LDFLAGS += -shared

# ---- define objects
GEN_OBJ     += susy.o main.o
EXE_OBJ     := $(GEN_OBJ) main.o

# PY_OBJ       := pyprep.o

ALLOBJ       := $(GEN_OBJ) $(PY_OBJ)

ALLOUTPUT    := main

all: $(ALLOUTPUT) 

main: $(EXE_OBJ:%=$(BIN)/%)
	@echo "linking $^ --> $@"
	@$(CXX) -o $@ $^ $(LIBS) $(LDFLAGS)

# pyprep.so: $(GEN_OBJ:%=$(BIN)/%) $(PY_OBJ:%=$(BIN)/%)
# 	@echo "linking $^ --> $@"
# 	@$(CXX) -o $@ $^ $(LIBS) $(PY_LDFLAGS)

# --------------------------------------------------

# python object compile
$(BIN)/py%.o: py%.cxx 
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
	rm -fr $(BIN) $(ALLOUTPUT)

rmdep: 
	rm -f $(DEP)/*.d