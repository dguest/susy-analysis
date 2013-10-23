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
  catch (std::runtime_error e) { 
    PyErr_SetString(PyExc_IOError, e.what()); 
    return NULL; 
  }
  PyObject* tuple = Py_BuildValue("i", ret_val);
  return tuple; 
}

static bool region_copy(PyObject* list, std::vector<RegionConfig>* regs) { 
  return safe_copy<RegionConfig>(list, regs); 
}


static bool safe_copy(PyObject* dict, RegionConfig& region)
{
  if (!PyDict_Check(dict)){
    PyErr_SetString(PyExc_ValueError, "Expected RegionConfig formatted dict");
    return false; 
  }
  if (!require(dict, "name", region.name)) return false; 
  if (!require(dict, "output_name", region.output_name)) return false;
  if (!require(dict, "type", region.type)) return false;
  
  if (!copy(dict, "required_bits", region.required_bits)) return false;
  if (!copy(dict, "veto_bits", region.veto_bits)) return false;
  if (!copy(dict, "region_bits", region.region_bits)) return false;
  if (!copy(dict, "systematic", region.systematic)) return false; 
  if (!copy(dict, "leading_jet_pt", region.leading_jet_pt)) return false;
  if (!copy(dict, "met", region.met)) return false; 
  if (!copy(dict, "jet_tag_requirements", 
	    region.jet_tag_requirements)) return false; 
  if (!copy(dict, "hists", region.hists)) return false; 
  if (!copy(dict, "mc_mc_jet_reweight_file", 
	    region.mc_mc_jet_reweight_file)) return false; 
  if (!copy(dict, "tagger", region.tagger)) return false; 
  if (!copy(dict, "jet_tag_assignment", 
	    region.jet_tag_assignment)) return false; 
  return true; 
}

static bool safe_copy(PyObject* value, std::string& dest){ 
  std::string cstr = PyString_AsString(value);
  if (PyErr_Occurred() || (PyString_Size(value) == 0)) return false; 
  dest = cstr; 
  return true; 
}

static bool safe_copy(PyObject* value, ull_t& dest) { 
  ull_t the_long = PyLong_AsUnsignedLongLong(value); 
  if (PyErr_Occurred()) return false; 
  dest = the_long; 
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
  NAME_TO_DEST(METUP); 
  NAME_TO_DEST(METDOWN); 

  std::string problem = "got undefined systematic: " + name; 
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
  if(strchr(flags,'i')) bitflags |= leading_jet_btag; 
  if(strchr(flags,'m')) bitflags |= mttop; 
  if(strchr(flags,'b')) bitflags |= disable_c_tags; 
  return bitflags; 
}
