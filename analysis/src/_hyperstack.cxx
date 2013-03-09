// python wrapper for the histo builder
// Author: Daniel Guest (dguest@cern.ch)
#include <Python.h>
// #include <string> 
// #include <vector>
// #include <map>
#include <stdexcept>

#include "HistBuilder.hh"
#include "HyperBuilder.hh"
#include "CutflowBuilder.hh"
#include "HistBuilderFlags.hh"
#include "_hyperstack.hh"
#include "HistConfig.hh"
#include "systematic_defs.hh"

static unsigned parse_flags(const char* flags);

template<typename T>
static PyObject* py_analysis_alg(PyObject *self, PyObject *args)
{
  const char* input_file = ""; 
  std::vector<RegionConfig> regions; 
  const char* flags = ""; 

  bool ok = PyArg_ParseTuple
    (args,"sO&|s:algo", &input_file, fill_regions, &regions, &flags); 
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
    PyErr_SetString(PyExc_RuntimeError, e.what()); 
    return NULL; 
  }

  PyObject* tuple = Py_BuildValue("i", ret_val);
  
  return tuple; 

}

static bool fill_regions(PyObject* list, std::vector<RegionConfig>* regions)
{
  const int n_regions = PyList_Size(list); 
  if (PyErr_Occurred()) return NULL; 
  for (int reg_n = 0; reg_n < n_regions; reg_n++) { 
    PyObject* entry = PyList_GetItem(list, reg_n); 
    RegionConfig region; 
    region.systematic = syst::NONE; 
    if (!fill_region(entry, &region)) return false; 
    regions->push_back(region); 
  }
  return true; 
}

static bool fill_region(PyObject* dict, RegionConfig* region)
{
  if (!dict) return false; 
  if (!PyDict_Check(dict)) return false; 
  if (!PyDict_Check(dict)) return false; 
  PyObject* key; 
  PyObject* value; 
  Py_ssize_t pos = 0; 
  while(PyDict_Next(dict, &pos, &key, &value)) { 
    if (!PyString_Check(key) || PyString_Size(key) == 0) { 
      PyErr_SetString(PyExc_ValueError, 
		      "config dict includes non-string key"); 
      return false; 
    }
    std::string ckey = PyString_AsString(key); 
    if (ckey == "name") { 
      if (!safe_copy(value, region->name)) return false; 
    }
    else if (ckey == "output_name") { 
      if (!safe_copy(value, region->output_name)) return false; 
    }
    else if (ckey == "jet_tag_requirements") { 
      if (!fill_jettag(value, &region->jet_tag_requirements)) return false;
    }
    else if (ckey == "leading_jet_pt") { 
      region->leading_jet_pt = PyFloat_AsDouble(value); 
      if (PyErr_Occurred()) return false; 
    }
    else if (ckey == "met") { 
      region->met = PyFloat_AsDouble(value); 
      if (PyErr_Occurred()) return false; 
    }
    else if (ckey == "required_bits") { 
      region->required_bits = PyLong_AsUnsignedLongLong(value); 
      if (PyErr_Occurred()) return false; 
    }
    else if (ckey == "veto_bits") { 
      region->veto_bits = PyLong_AsUnsignedLongLong(value); 
      if (PyErr_Occurred()) return false; 
    }
    else if (ckey == "systematic") { 
      if (!safe_copy(value, region->systematic)) return false; 
    }
    else { 
      std::string problem = "got unknown signal region option: " + ckey; 
      PyErr_SetString(PyExc_ValueError,problem.c_str()); 
      return false; 
    }
  }
  return true; 
}  


static bool fill_jettag(PyObject* list, std::vector<btag::JetTag>* req) { 
  const int n_tags = PyList_Size(list); 
  if (PyErr_Occurred()) return NULL; 
  for (int tag_n = 0; tag_n < n_tags; tag_n++) { 
    PyObject* entry = PyList_GetItem(list, tag_n); 
    btag::JetTag tag = btag::NOTAG; 
    if (!safe_copy(entry, tag)) return false; 
    req->push_back(tag); 
  }
  return true; 
}

static bool safe_copy(PyObject* value, btag::JetTag& dest) { 
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
  else { 
    std::string problem = "got undefined btag: " + name; 
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

static bool safe_copy(PyObject* value, std::string& dest){ 
  std::string cstr = PyString_AsString(value);
  if (PyErr_Occurred() || (PyString_Size(value) == 0)) return false; 
  dest = cstr; 
  return true; 
}


static PyMethodDef methods[] = {
  {"_stacksusy", py_analysis_alg<HistBuilder>, METH_VARARGS, 
   "don't ask, read the source"},
  // {"_hypersusy", py_analysis_alg<HyperBuilder>, METH_VARARGS, 
  //  "eat a failure sandwich"}, 
  // {"_cutflow", py_analysis_alg<CutflowBuilder>, METH_VARARGS, 
  //  "eat a failure sandwich"}, 
  {NULL, NULL, 0, NULL}   /* sentinel */
};

extern "C" { 

  PyMODINIT_FUNC init_hyperstack(void)
  {
    Py_InitModule("_hyperstack", methods);
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
