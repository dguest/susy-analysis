// python wrapper for the histo builder
// Author: Daniel Guest (dguest@cern.ch)
#include <Python.h>
#include <stdexcept>

#include "HistBuilder.hh"
#include "HistBuilderFlags.hh"
#include "_hfw.hh"
#include "systematic_defs.hh"

static unsigned parse_flags(const char* flags);

// not sure why this can't be templated...
static bool region_copy(PyObject* list, std::vector<RegionConfig>* regs); 

template<typename T>
static PyObject* py_analysis_alg(PyObject *self, PyObject *args)
{
  const char* input_file = ""; 
  std::vector<RegionConfig> regions; 
  const char* flags = ""; 

  bool ok = PyArg_ParseTuple
    (args,"sO&|s:algo", &input_file, region_copy, &regions, &flags); 
  if (!ok) return NULL;

  unsigned bitflags = parse_flags(flags); 
  int ret_val = 0; 
  try { 
    T builder(input_file, bitflags); 
    for (auto itr = regions.begin(); itr != regions.end(); itr++) {
      builder.add_region(*itr); 
    }
    ret_val = builder.build(); 
    builder.save(); 
  }
  catch (std::runtime_error& e) { 
    PyErr_SetString(PyExc_IOError, e.what()); 
    return NULL; 
  }
  catch (std::logic_error& e) { 
    PyErr_SetString(PyExc_StandardError, e.what()); 
    return NULL; 
  }
  PyObject* tuple = Py_BuildValue("i", ret_val);
  return tuple; 
}

// _______________________________________________________________
// top level copy commands

static bool region_copy(PyObject* list, std::vector<RegionConfig>* regs) { 
  return safe_copy<RegionConfig>(list, regs); 
}


static bool safe_copy(PyObject* dict, RegionConfig& region)
{
  if (!PyDict_Check(dict)){
    PyErr_SetString(PyExc_ValueError, "Expected RegionConfig formatted dict");
    return false; 
  }
#define REQUIRE(symbol)						\
  if (!require(dict, #symbol, region.symbol)) return false
#define COPY(symbol)						\
  if (!copy(dict, #symbol, region.symbol)) return false

  REQUIRE(name);
  REQUIRE(selection); 
  REQUIRE(output_name); 
  REQUIRE(type); 
  
  COPY(region_bits); 
  COPY(systematic); 
  COPY(leading_jet_pt); 
  COPY(met);
  COPY(jet_tag_requirements); 
  COPY(hists); 
  COPY(mc_mc_jet_reweight_file); 
  COPY(tagger); 
  COPY(jet_tag_assignment); 
  COPY(boson_pt_correction); 

#undef REQUIRE
#undef COPY

  return true; 
}

// _______________________________________________________________
// basic copy types

static bool safe_copy(PyObject* value, std::string& dest){ 
  std::string cstr = PyString_AsString(value);
  if (PyErr_Occurred() || (PyString_Size(value) == 0)) return false; 
  dest = cstr; 
  return true; 
}

static bool safe_copy(PyObject* value, unsigned& dest) { 
  unsigned the_long = PyLong_AsUnsignedLong(value); 
  if (PyErr_Occurred()) return false; 
  dest = the_long; 
  return true; 
}

static bool safe_copy(PyObject* value, double& dest) { 
  double the_double = PyFloat_AsDouble(value); 
  if (PyErr_Occurred()) return false; 
  dest = the_double; 
  return true; 
}

// _______________________________________________________________
// enum converter copy commands

// for things in some namespace / of type enum class
#define NAME_TO_PREFIXED(PREFIX, NAME) \
  do { if (name == #NAME) {	       \
      dest = PREFIX::NAME;	       \
      return true;		       \
    }				       \
  } while (0)


static bool safe_copy(PyObject* value, reg::Selection& dest) { 
  char* charname = PyString_AsString(value); 
  if (PyErr_Occurred()) return false; 
  std::string name(charname); 
  using namespace reg; 

  NAME_TO_PREFIXED(Selection, SIGNAL); 
  NAME_TO_PREFIXED(Selection, CR_1L); 
  NAME_TO_PREFIXED(Selection, CR_SF); 
  NAME_TO_PREFIXED(Selection, CR_DF); 
  NAME_TO_PREFIXED(Selection, NONE); 

  std::string problem = "got undefined selection: " + name; 
  PyErr_SetString(PyExc_ValueError,problem.c_str()); 
  return false; 
}


#define NAME_TO_DEST(NAME)		\
  do { if (name == #NAME) { \
    dest = NAME; \
    return true; \
    }		 \
  } while (0)


static bool safe_copy(PyObject* value, btag::OperatingPoint& dest) { 
  char* charname = PyString_AsString(value); 
  if (PyErr_Occurred()) return false; 
  std::string name(charname); 
  using namespace btag; 

  NAME_TO_DEST(NOTAG); 
  NAME_TO_DEST(LOOSE); 
  NAME_TO_DEST(MEDIUM); 
  NAME_TO_DEST(TIGHT); 
  NAME_TO_DEST(ANTILOOSE); 

  NAME_TO_DEST(JFC_LOOSE); 
  NAME_TO_DEST(JFC_MEDIUM); 
  NAME_TO_DEST(JFC_TIGHT); 
  NAME_TO_DEST(JFC_ANTILOOSE); 

  std::string problem = "got undefined op: " + name; 
  PyErr_SetString(PyExc_ValueError,problem.c_str()); 
  return false; 
}

static bool safe_copy(PyObject* value, btag::Tagger& dest) { 
  char* charname = PyString_AsString(value); 
  if (PyErr_Occurred()) return false; 
  std::string name(charname); 
  using namespace btag; 

  NAME_TO_DEST(CNN); 
  NAME_TO_DEST(JFC); 

  std::string problem = "got undefined tagger: " + name; 
  PyErr_SetString(PyExc_ValueError,problem.c_str()); 
  return false; 
}

static bool safe_copy(PyObject* value, btag::Assignment& dest) { 
  char* charname = PyString_AsString(value); 
  if (PyErr_Occurred()) return false; 
  std::string name(charname); 
  using namespace btag; 

  NAME_TO_DEST(PT_ORDERED); 
  NAME_TO_DEST(TAG_ORDERED); 

  std::string problem = "got undefined tagging assignment: " + name; 
  PyErr_SetString(PyExc_ValueError,problem.c_str()); 
  return false; 
}


static bool safe_copy(PyObject* value, reg::Type& dest) { 
  char* charname = PyString_AsString(value); 
  if (PyErr_Occurred()) return false; 
  std::string name(charname); 
  using namespace reg; 

  NAME_TO_DEST(CONTROL); 
  NAME_TO_DEST(SIGNAL); 
  NAME_TO_DEST(VALIDATION); 

  std::string problem = "got undefined region type: " + name; 
  PyErr_SetString(PyExc_ValueError,problem.c_str()); 
  return false; 
}

static bool safe_copy(PyObject* value, reg::Hists& dest) { 
  char* charname = PyString_AsString(value); 
  if (PyErr_Occurred()) return false; 
  std::string name(charname); 
  using namespace reg; 

  NAME_TO_DEST(HISTMILL); 
  NAME_TO_DEST(KINEMATIC_STAT); 
  NAME_TO_DEST(TAG_EFFICIENCY); 
  NAME_TO_DEST(BOSON_PT); 

  std::string problem = "got undefined hists type: " + name; 
  PyErr_SetString(PyExc_ValueError,problem.c_str()); 
  return false; 
}

static bool safe_copy(PyObject* value, syst::Systematic& dest) { 
  char* charname = PyString_AsString(value); 
  if (PyErr_Occurred()) return false; 
  std::string name(charname); 
  using namespace syst; 
  
  NAME_TO_DEST(NONE); 
  NAME_TO_DEST(BUP); 
  NAME_TO_DEST(BDOWN); 
  NAME_TO_DEST(CUP); 
  NAME_TO_DEST(CDOWN); 
  NAME_TO_DEST(UUP); 
  NAME_TO_DEST(UDOWN); 
  NAME_TO_DEST(TUP);
  NAME_TO_DEST(TDOWN); 
  NAME_TO_DEST(ELUP); 
  NAME_TO_DEST(ELDOWN); 
  NAME_TO_DEST(MUUP); 
  NAME_TO_DEST(MUDOWN); 

  std::string problem = "got undefined systematic: " + name; 
  PyErr_SetString(PyExc_ValueError,problem.c_str()); 
  return false; 
}

static bool safe_copy(PyObject* value, reg::BosonPtCorrection& dest) {
  char* charname = PyString_AsString(value); 
  if (PyErr_Occurred()) return false; 
  std::string name(charname); 
  using namespace reg; 
  NAME_TO_DEST(MARKS); 
  NAME_TO_DEST(NO_PT_CORRECTION); 
  std::string problem = "got undefined boson correction: " + name; 
  PyErr_SetString(PyExc_ValueError,problem.c_str()); 
  return false; 
}

static PyMethodDef methods[] = {
  {"_stacksusy", py_analysis_alg<HistBuilder>, METH_VARARGS, 
   "don't ask, read the source"},
  {NULL, NULL, 0, NULL}   /* sentinel */
};

extern "C" { 

  PyMODINIT_FUNC init_hfw(void)
  {
    Py_InitModule("_hfw", methods);
  }

}


static unsigned parse_flags(const char* flags){ 
  using namespace buildflag; 
  unsigned bitflags = 0; 
  if(strchr(flags,'v')) bitflags |= verbose; 
  if(strchr(flags,'t')) bitflags |= fill_truth; 
  if(strchr(flags,'d')) bitflags |= is_data; 
  return bitflags; 
}
