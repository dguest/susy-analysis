// python wrapper for the histo builder
// Author: Daniel Guest (dguest@cern.ch)
#include <Python.h>

#include "HistBuilder.hh"
#include "HistBuilderFlags.hh"
#include "_hfw.hh"
#include "systematic_defs.hh"

#include <set>

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
  catch (const std::runtime_error& e) { 
    PyErr_SetString(PyExc_IOError, e.what()); 
    return NULL; 
  }
  catch (const std::logic_error& e) { 
    PyErr_SetString(PyExc_Exception, e.what()); 
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
  REQUIRE(stream);
  
  COPY(systematic); 
  COPY(leading_jet_pt); 
  COPY(met);
  COPY(jet_tag_requirements); 
  COPY(hists); 
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
  std::string cstr = PyUnicode_AsUTF8(value);
  if (PyErr_Occurred() || (PyUnicode_GetLength(value) == 0)) return false; 
  dest = cstr; 
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
  char* charname = PyUnicode_AsUTF8(value); 
  if (PyErr_Occurred()) return false; 
  std::string name(charname); 
  using namespace reg; 

  NAME_TO_PREFIXED(Selection, SIGNAL); 
  NAME_TO_PREFIXED(Selection, CR_1L); 
  NAME_TO_PREFIXED(Selection, CR_SF); 
  NAME_TO_PREFIXED(Selection, CR_DF); 
  NAME_TO_PREFIXED(Selection, QUALITY_EVENT); 

  std::string problem = "got undefined selection: " + name; 
  PyErr_SetString(PyExc_ValueError,problem.c_str()); 
  return false; 
}

static bool safe_copy(PyObject* value, reg::Stream& dest) { 
  char* charname = PyUnicode_AsUTF8(value); 
  if (PyErr_Occurred()) return false; 
  std::string name(charname); 
  using namespace reg; 

  NAME_TO_PREFIXED(Stream, ELECTRON);
  NAME_TO_PREFIXED(Stream, MUON);
  NAME_TO_PREFIXED(Stream, JET);

  // mc streams don't require any special action
  const std::set<std::string> mc_streams{"ATLFAST", "FULLSIM"};
  if (mc_streams.count(name)) { 
    dest = Stream::SIMULATED;
    return true;
  }

  std::string problem = "got undefined stream: " + name; 
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
  char* charname = PyUnicode_AsUTF8(value); 
  if (PyErr_Occurred()) return false; 
  std::string name(charname); 
  using namespace btag; 

  NAME_TO_PREFIXED(OperatingPoint, NOTAG); 
  NAME_TO_PREFIXED(OperatingPoint, LOOSE); 
  NAME_TO_PREFIXED(OperatingPoint, MEDIUM); 
  NAME_TO_PREFIXED(OperatingPoint, TIGHT); 
  NAME_TO_PREFIXED(OperatingPoint, ANTILOOSE); 

  NAME_TO_PREFIXED(OperatingPoint, JFC_LOOSE); 
  NAME_TO_PREFIXED(OperatingPoint, JFC_MEDIUM); 
  NAME_TO_PREFIXED(OperatingPoint, JFC_TIGHT); 
  NAME_TO_PREFIXED(OperatingPoint, JFC_ANTILOOSE); 

  std::string problem = "got undefined op: " + name; 
  PyErr_SetString(PyExc_ValueError,problem.c_str()); 
  return false; 
}

static bool safe_copy(PyObject* value, btag::Tagger& dest) { 
  char* charname = PyUnicode_AsUTF8(value); 
  if (PyErr_Occurred()) return false; 
  std::string name(charname); 
  using namespace btag; 

  NAME_TO_PREFIXED(Tagger, CNN); 
  NAME_TO_PREFIXED(Tagger, JFC); 

  std::string problem = "got undefined tagger: " + name; 
  PyErr_SetString(PyExc_ValueError,problem.c_str()); 
  return false; 
}

static bool safe_copy(PyObject* value, btag::Assignment& dest) { 
  char* charname = PyUnicode_AsUTF8(value); 
  if (PyErr_Occurred()) return false; 
  std::string name(charname); 
  using namespace btag; 

  NAME_TO_PREFIXED(Assignment, PT_ORDERED); 
  NAME_TO_PREFIXED(Assignment, TAG_ORDERED); 

  std::string problem = "got undefined tagging assignment: " + name; 
  PyErr_SetString(PyExc_ValueError,problem.c_str()); 
  return false; 
}


static bool safe_copy(PyObject* value, reg::Type& dest) { 
  char* charname = PyUnicode_AsUTF8(value); 
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
  char* charname = PyUnicode_AsUTF8(value); 
  if (PyErr_Occurred()) return false; 
  std::string name(charname); 
  using namespace reg; 

  NAME_TO_DEST(HISTMILL); 
  NAME_TO_DEST(KINEMATIC_STAT); 
  NAME_TO_DEST(TAG_EFFICIENCY); 
  NAME_TO_DEST(BOSON_PT); 
  NAME_TO_DEST(NMINUS); 

  std::string problem = "got undefined hists type: " + name; 
  PyErr_SetString(PyExc_ValueError,problem.c_str()); 
  return false; 
}

static bool safe_copy(PyObject* value, syst::Systematic& dest) { 
  char* charname = PyUnicode_AsUTF8(value); 
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
  char* charname = PyUnicode_AsUTF8(value); 
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

static struct PyModuleDef hfw  = {
   PyModuleDef_HEAD_INIT,
   "hfw",   /* name of module */
   "this be hfw", /* module documentation, may be NULL */
   -1,       /* size of per-interpreter state of the module,
                or -1 if the module keeps state in global variables. */
   methods
};

extern "C" { 
  PyMODINIT_FUNC PyInit__hfw(void)
  {
    return PyModule_Create(&hfw);
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
