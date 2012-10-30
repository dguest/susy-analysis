// python wrapper for the histo builder
// Author: Daniel Guest (dguest@cern.ch)
#include <Python.h>
// #include <string> 
// #include <vector>
// #include <map>
#include <stdexcept>

#include "HistBuilder.hh"

static PyObject* py_stacksusy(PyObject *self, PyObject *args)
{
  const char* input_file = ""; 
  PyObject* bits = 0; 
  const char* output_file = ""; 
  const char* flags = ""; 

  bool ok = PyArg_ParseTuple
    (args,"sOs|s:stacksusy", 
     &input_file, &bits, &output_file, &flags); 
  if (!ok) return NULL;

  try { 
    HistBuilder hist_builder(input_file); 

    const int n_bits = PyList_Size(bits); 
    if (PyErr_Occurred()) return NULL; 
    for (int bit_n = 0; bit_n < n_bits; bit_n++) { 
      PyObject* entry = PyList_GetItem(bits, bit_n); 
      const char* name = ""; 
      unsigned mask = 0; 
      ok = PyArg_ParseTuple(entry, "sk:parsemask", &name, &mask); 
      if (!ok) return NULL; 
      hist_builder.add_cut_mask(name, mask); 
    }
    
    hist_builder.build(); 
    hist_builder.save(output_file); 
  }
  catch (std::runtime_error e) { 
    PyErr_SetString(PyExc_RuntimeError, e.what()); 
    return NULL; 
  }

  PyObject* tuple = Py_BuildValue("s", input_file);
  
  return tuple; 

}

static PyMethodDef methods[] = {
  {"_stacksusy", py_stacksusy, METH_VARARGS, 
   "don't ask, read the source"},
  {NULL, NULL, 0, NULL}   /* sentinel */
};

extern "C" { 

  PyMODINIT_FUNC init_hyperstack(void)
  {
    Py_InitModule("_hyperstack", methods);
  }

}
