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
  PyObject* py_input_files; 
  const char* flags_str = ""; 
  const char* out_ntuple = ""; 

  RunInfo info; 
  bool ok = PyArg_ParseTuple
    (args,"Oi|ss:cutflow", &py_input_files, &info.run_number, 
     &flags_str, &out_ntuple); 
  if (!ok) return NULL;

  int n_files = PyList_Size(py_input_files); 
  if (PyErr_Occurred()) return NULL; 
    
  std::vector<std::string> input_files; 
  for (int n = 0; n < n_files; n++) { 
    PyObject* py_file_name = PyList_GetItem(py_input_files, n); 
    std::string file_name = PyString_AsString(py_file_name); 
    if (PyErr_Occurred()) return NULL; 
    input_files.push_back(file_name);     
  }
  
  unsigned flags = 0; 
  if (strchr(flags_str,'v')) flags |= cutflag::verbose; 
  if (strchr(flags_str,'d')) flags |= cutflag::is_data; 
  if (strchr(flags_str,'s')) flags |= cutflag::is_signal; 
  if (strchr(flags_str,'p')) flags |= cutflag::use_low_pt_jets; 
  if (strchr(flags_str,'d')) flags |= cutflag::debug_susy; 

  typedef std::vector<std::pair<std::string, int> > CCOut; 
  CCOut pass_numbers; 
  try { 
    pass_numbers = run_cutflow(input_files, info, flags, out_ntuple); 
  }
  catch (std::runtime_error e) { 
    PyErr_SetString(PyExc_RuntimeError, e.what()); 
    return NULL; 
  }

  PyObject* out_list = PyList_New(0); 
  for (CCOut::const_iterator itr = pass_numbers.begin(); 
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
