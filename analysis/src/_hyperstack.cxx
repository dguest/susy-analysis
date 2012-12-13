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

static unsigned parse_flags(const char* flags);

template<typename T>
static PyObject* py_analysis_alg(PyObject *self, PyObject *args)
{
  const char* input_file = ""; 
  PyObject* bits = 0; 
  const char* output_file = ""; 
  const char* flags = ""; 
  PyObject* floats_dict = 0; 

  bool ok = PyArg_ParseTuple
    (args,"sOs|sO:algo", 
     &input_file, &bits, &output_file, &flags, &floats_dict); 
  if (!ok) return NULL;

  unsigned bitflags = parse_flags(flags); 

  int ret_val = 0; 
  try { 
    T builder(input_file, bitflags); 

    const int n_bits = PyList_Size(bits); 
    if (PyErr_Occurred()) return NULL; 
    for (int bit_n = 0; bit_n < n_bits; bit_n++) { 
      PyObject* entry = PyList_GetItem(bits, bit_n); 
      const char* name = ""; 
      unsigned mask = 0; 
      ok = PyArg_ParseTuple(entry, "sk:parsemask", &name, &mask); 
      if (!ok) return NULL; 
      builder.add_cut_mask(name, mask); 
    }
    PyObject* dic_key = 0; 
    PyObject* dic_val = 0; 
    int pos = 0; 
    if (floats_dict) { 
      while (PyDict_Next(floats_dict, &pos, &dic_key, &dic_val)) { 
	char* key_val = PyString_AsString(dic_key); 
	if (PyErr_Occurred()) return NULL; 
	float val_val = PyFloat_AsDouble(dic_val); 
	if (PyErr_Occurred()) return NULL; 
	builder.set_float(key_val, val_val); 
      }
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
  if(strchr(flags,'i')) bitflags |= leading_jet_btag; 
  if(strchr(flags,'m')) bitflags |= mttop; 
  if(strchr(flags,'b')) bitflags |= disable_c_tags; 
  return bitflags; 
}
