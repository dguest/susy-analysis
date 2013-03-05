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
  PyObject* bits = 0; 
  const char* output_file = ""; 
  const char* flags = ""; 
  HistConfig hist_config; 

  bool ok = PyArg_ParseTuple
    (args,"sOs|sO&:algo", 
     &input_file, &bits, &output_file, &flags, fill_config, &hist_config); 
  if (!ok) return NULL;

  unsigned bitflags = parse_flags(flags); 

  int ret_val = 0; 
  try { 
    T builder(input_file, hist_config, bitflags); 

    const int n_bits = PyList_Size(bits); 
    if (PyErr_Occurred()) return NULL; 
    for (int bit_n = 0; bit_n < n_bits; bit_n++) { 
      PyObject* entry = PyList_GetItem(bits, bit_n); 
      const char* name = ""; 
      unsigned mask = 0; 
      unsigned antimask = 0; 
      ok = PyArg_ParseTuple(entry, "sk|k:parsemask", &name, 
			    &mask, &antimask); 
      if (!ok) return NULL; 
      builder.add_cut_mask(name, mask, antimask); 
    }
    
    ret_val = builder.build(); 
    builder.save(output_file); 
  }
  catch (std::runtime_error e) { 
    PyErr_SetString(PyExc_RuntimeError, e.what()); 
    return NULL; 
  }

  PyObject* tuple = Py_BuildValue("i", ret_val);
  
  return tuple; 

}

static bool fill_config(PyObject* dict, HistConfig* config) { 
  if (!dict) return false; 
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
    if (ckey == "btag_config") { 
      if (!safe_copy(value, config->btag_config)) return false; 
    }
    else if (ckey == "systematic") { 
      if (!safe_copy(value, config->systematic)) return false; 
    }
    else { 
      if (!safe_copy(ckey, value, config->floats)) return false; 
    }
  }
  return true; 
}

static bool safe_copy(std::string key, 
		      PyObject* value, 
		      std::map<std::string, float>& fmap) { 
  if (fmap.count(key) != 0) { 
    std::string problem = "tried to redefine key: " + key; 
    PyErr_SetString(PyExc_ValueError,problem.c_str()); 
    return false; 
  }
  double the_double = PyFloat_AsDouble(value); 
  if (PyErr_Occurred()) {
    std::string problem = "wanted a float for " + key; 
    PyErr_SetString(PyExc_TypeError, problem.c_str()); 
    return false; 
  }
  fmap[key] = the_double; 
  return true; 
}

static bool safe_copy(PyObject* value, btag::EventConfig& dest) { 
  char* charname = PyString_AsString(value); 
  if (PyErr_Occurred()) return false; 
  std::string name(charname); 
  using namespace btag; 
  if (name == "LOOSE_TIGHT") { 
    dest = LOOSE_TIGHT; 
    return true; 
  }
  else if (name == "MEDIUM_MEDIUM") { 
    dest = MEDIUM_MEDIUM; 
    return true; 
  }
  else if (name == "MEDIUM_TIGHT") { 
    dest = MEDIUM_TIGHT; 
    return true; 
  }
  else if (name == "NONE") { 
    dest = NONE; 
    return true; 
  }
  else { 
    std::string problem = "got undefined btag configuration: " + name; 
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
  {"_hypersusy", py_analysis_alg<HyperBuilder>, METH_VARARGS, 
   "eat a failure sandwich"}, 
  {"_cutflow", py_analysis_alg<CutflowBuilder>, METH_VARARGS, 
   "eat a failure sandwich"}, 
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
