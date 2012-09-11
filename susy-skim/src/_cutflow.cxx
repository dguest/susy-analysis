// python wrapper for susy cutflow
// Author: Daniel Guest (dguest@cern.ch)
#include <Python.h>
#include <string> 
#include <vector>
#include <map>
#include <stdexcept>

#include "cutflow.hh"

static PyObject* py_cutflow(PyObject *self, 
			    PyObject *args)
{
  const char* input_file; 
  const char* flags_str = ""; 
    
  bool ok = PyArg_ParseTuple(args,"s|s:cutflow", &input_file, &flags_str); 
  if (!ok) return NULL;

  std::vector<std::string> input_files; 
  input_files.push_back(input_file); 
  
  unsigned flags = 0; 
  if (strchr(flags_str,'v')) flags |= cutflag::verbose; 

  RunInfo info; 
  info.is_data = false; 
  info.run_number = 180614; 
  info.is_signal = true; 

  std::map<std::string, int> pass_numbers; 
  try { 
    pass_numbers = run_cutflow(input_files, info, flags); 
  }
  catch (std::runtime_error e) { 
    PyErr_SetString(PyExc_RuntimeError, e.what()); 
    return NULL; 
  }

  PyObject* out_dict = PyDict_New(); 
  for (std::map<std::string, int>::const_iterator itr = pass_numbers.begin(); 
       itr != pass_numbers.end(); 
       itr++){ 
    PyObject* count = PyInt_FromLong(itr->second); 
    if(PyDict_SetItemString(out_dict,itr->first.c_str(),count)) { 
      return NULL; 
    }
  }
  
  return out_dict; 

}

static PyMethodDef methods[] = {
  {"_cutflow", py_cutflow, METH_VARARGS, 
   "cutflow(input_file, flags) --> cuts_dict\n"
   "flags:\n"
   "\tv: verbose\n"},
  {NULL, NULL, 0, NULL}   /* sentinel */
};

extern "C" { 

  PyMODINIT_FUNC init_cutflow(void)
  {
    Py_InitModule("_cutflow", methods);
  }

}
