// python wrapper for the histo builder
// Author: Daniel Guest (dguest@cern.ch)
#include <Python.h>
// #include <string> 
// #include <vector>
// #include <map>
#include <stdexcept>

// #include "cutflow.hh"
// #include "CutBits.hh"
// #include "RunInfo.hh"

static PyObject* py_hist_from_whiskey(PyObject *self, 
				      PyObject *args)
{
  const char* input_file = ""; 

  bool ok = PyArg_ParseTuple(args,"s:hist_from_whiskey", &input_file); 
  if (!ok) return NULL;

  try { 
    printf(input_file); 
  }
  catch (std::runtime_error e) { 
    PyErr_SetString(PyExc_RuntimeError, e.what()); 
    return NULL; 
  }

  PyObject* tuple = Py_BuildValue("s", input_file);
  
  return tuple; 

}

static PyMethodDef methods[] = {
  {"_hist_from_whiskey", py_hist_from_whiskey, METH_VARARGS, 
   "don't ask, read the source"},
  {NULL, NULL, 0, NULL}   /* sentinel */
};

extern "C" { 

  PyMODINIT_FUNC init_hist_from_whiskey(void)
  {
    Py_InitModule("_hist_from_whiskey", methods);
  }

}
