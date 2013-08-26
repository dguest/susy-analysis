// python wrapper for the histo builder
// Author: Daniel Guest (dguest@cern.ch)
#include <Python.h>
#include <stdexcept>

#include "HistBuilder.hh"
#include "HistBuilderFlags.hh"
#include "_hfw.hh"
// #include "HistConfig.hh"
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
  if (name == "NOTAG") { 
    dest = NOTAG; 
    return true; 
  }
  else if (name == "LOOSE") { 
    dest = LOOSE; 
    return true; 
  }
  else if (name == "MEDIUM") { 
    dest = MEDIUM; 
    return true; 
  }
  else if (name == "TIGHT") { 
    dest = TIGHT; 
    return true; 
  }
  else if (name == "ANTILOOSE") { 
    dest = ANTILOOSE; 
    return true; 
  }

  else if (name == "JFC_LOOSE") { 
    dest = JFC_LOOSE; 
    return true; 
  }
  else if (name == "JFC_MEDIUM") { 
    dest = JFC_MEDIUM; 
    return true; 
  }
  else if (name == "JFC_TIGHT") { 
    dest = JFC_TIGHT; 
    return true; 
  }
  else if (name == "JFC_ANTILOOSE") { 
    dest = JFC_ANTILOOSE; 
    return true; 
  }
  else { 
    std::string problem = "got undefined op: " + name; 
    PyErr_SetString(PyExc_ValueError,problem.c_str()); 
    return false; 
  }
}

static bool safe_copy(PyObject* value, btag::Tagger& dest) { 
  char* charname = PyString_AsString(value); 
  if (PyErr_Occurred()) return false; 
  std::string name(charname); 
  using namespace btag; 
  if (name == "CNN") { 
    dest = CNN; 
    return true; 
  }
  else if (name == "JFC") { 
    dest = JFC; 
    return true; 
  }
  else { 
    std::string problem = "got undefined tagger: " + name; 
    PyErr_SetString(PyExc_ValueError,problem.c_str()); 
    return false; 
  }
}

static bool safe_copy(PyObject* value, btag::Assignment& dest) { 
  char* charname = PyString_AsString(value); 
  if (PyErr_Occurred()) return false; 
  std::string name(charname); 
  using namespace btag; 
  if (name == "PT_ORDERED") { 
    dest = PT_ORDERED; 
    return true; 
  }
  else if (name == "TAG_ORDERED") { 
    dest = TAG_ORDERED; 
    return true; 
  }
  else { 
    std::string problem = "got undefined tagging assignment: " + name; 
    PyErr_SetString(PyExc_ValueError,problem.c_str()); 
    return false; 
  }
}


static bool safe_copy(PyObject* value, reg::Type& dest) { 
  char* charname = PyString_AsString(value); 
  if (PyErr_Occurred()) return false; 
  std::string name(charname); 
  using namespace reg; 
  if (name == "CONTROL") { 
    dest = CONTROL; 
    return true; 
  }
  else if (name == "SIGNAL") { 
    dest = SIGNAL; 
    return true; 
  }
  else if (name == "VALIDATION") { 
    dest = VALIDATION; 
    return true; 
  }
  else { 
    std::string problem = "got undefined region type: " + name; 
    PyErr_SetString(PyExc_ValueError,problem.c_str()); 
    return false; 
  }
}

static bool safe_copy(PyObject* value, reg::Hists& dest) { 
  char* charname = PyString_AsString(value); 
  if (PyErr_Occurred()) return false; 
  std::string name(charname); 
  using namespace reg; 
  if (name == "HISTMILL") { 
    dest = HISTMILL; 
    return true; 
  }
  else if (name == "KINEMATIC_STAT") { 
    dest = KINEMATIC_STAT; 
    return true; 
  }
  else if (name == "TAG_EFFICIENCY") { 
    dest = TAG_EFFICIENCY; 
    return true; 
  }
  else { 
    std::string problem = "got undefined hists type: " + name; 
    PyErr_SetString(PyExc_ValueError,problem.c_str()); 
    return false; 
  }

}

static bool safe_copy(PyObject* value, syst::Systematic& dest) { 
  char* charname = PyString_AsString(value); 
  if (PyErr_Occurred()) return false; 
  std::string name(charname); 
  using namespace syst; 
  if (name == "NONE") { 
    dest = NONE; 
    return true; 
  }
  if (name == "BUP") { 
    dest = BUP; 
    return true; 
  }
  if (name == "BDOWN") { 
    dest = BDOWN; 
    return true; 
  }
  if (name == "CUP") { 
    dest = CUP; 
    return true; 
  }
  if (name == "CDOWN") { 
    dest = CDOWN; 
    return true; 
  }
  if (name == "UUP") { 
    dest = UUP; 
    return true; 
  }
  if (name == "UDOWN") { 
    dest = UDOWN; 
    return true; 
  }
  if (name == "TUP") { 
    dest = TUP; 
    return true; 
  }
  if (name == "TDOWN") { 
    dest = TDOWN; 
    return true; 
  }
  else { 
    std::string problem = "got undefined systematic: " + name; 
    PyErr_SetString(PyExc_ValueError,problem.c_str()); 
    return false; 
  }
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
