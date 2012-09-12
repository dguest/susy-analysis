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

  RunInfo info; 
  bool ok = PyArg_ParseTuple
    (args,"si|s:cutflow", &input_file, &info.run_number, &flags_str); 
  if (!ok) return NULL;

  std::vector<std::string> input_files; 
  input_files.push_back(input_file); 
  
  unsigned flags = 0; 
  if (strchr(flags_str,'v')) flags |= cutflag::verbose; 
  if (strchr(flags_str,'d')) flags |= cutflag::is_data; 
  if (strchr(flags_str,'s')) flags |= cutflag::is_signal; 

  std::map<std::string, int> pass_numbers; 
  try { 
    pass_numbers = run_cutflow(input_files, info, flags); 
  }
  catch (std::runtime_error e) { 
    PyErr_SetString(PyExc_RuntimeError, e.what()); 
    return NULL; 
  }

  PyObject* out_list = PyList_New(0); 
  for (std::map<std::string, int>::const_iterator itr = pass_numbers.begin(); 
       itr != pass_numbers.end(); 
       itr++){ 
    PyObject* tuple = Py_BuildValue("si", itr->first.c_str(), itr->second);
    if(PyList_Append(out_list,tuple)) { 
      return NULL; 
    }
  }
  
  return out_list; 

}

static PyMethodDef methods[] = {
  {"_cutflow", py_cutflow, METH_VARARGS, 
   "cutflow(input_file, flags) --> cuts_list\n"
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
