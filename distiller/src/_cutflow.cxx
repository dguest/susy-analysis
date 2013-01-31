// python wrapper for susy cutflow / d3pd reader
// Author: Daniel Guest (dguest@cern.ch)
#include <Python.h>
#include <string> 
#include <vector>
#include <map>
#include <stdexcept>

#include "cutflow.hh"
#include "RunBits.hh"
#include "PerformanceNtuple.hh"
#include "RunInfo.hh"

static PyObject* py_cutflow(PyObject *self, 
			    PyObject *args)
{
  PyObject* py_input_files; 
  const char* flags_str = ""; 
  const char* grl_char = ""; 
  const char* out_ntuple = ""; 

  RunInfo info; 

  bool ok = PyArg_ParseTuple
    (args,"Oi|sss:cutflow", &py_input_files, &info.run_number, 
     &flags_str, &grl_char, &out_ntuple); 
  if (!ok) return NULL;
  info.grl = strlen(grl_char) ? grl_char : std::string();  

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
  if (strchr(flags_str,'b')) flags |= cutflag::debug_susy; 
  if (strchr(flags_str,'r')) flags |= cutflag::save_ratios; 
  if (strchr(flags_str,'f')) flags |= cutflag::is_atlfast; 
  if (strchr(flags_str,'c')) flags |= cutflag::jetfitter_charm; 
  if (strchr(flags_str,'m')) flags |= cutflag::mv3; 

  // other taggers not implemented yet
  assert( (flags & (cutflag::jetfitter_charm | cutflag::mv3)) == 0); 

  typedef std::vector<std::pair<std::string, int> > CCOut; 
  CCOut pass_numbers; 
  try { 
    pass_numbers = run_cutflow(input_files, info, flags, out_ntuple); 
  }
  catch (std::runtime_error e) { 
    PyErr_SetString(PyExc_RuntimeError, e.what()); 
    return NULL; 
  }
  catch (...) { 
    PyErr_SetString(PyExc_Exception, "unknown exception"); 
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

static PyObject* py_perf_ntuple(PyObject *self, 
				PyObject *args)
{
  const char* input_file = ""; 
  const char* flags_str = ""; 
  const char* out_ntuple = ""; 

  bool ok = PyArg_ParseTuple
    (args,"s|ss:cutflow", &input_file, &flags_str, &out_ntuple); 
  if (!ok) return NULL;
  
  unsigned flags = 0; 
  if (strchr(flags_str,'c')) flags |= cutflag::jetfitter_charm; 
  if (strchr(flags_str,'m')) flags |= cutflag::mv3; 

  int return_code = 0; 
  try { 
    return_code = make_perf_ntuple(input_file, flags, out_ntuple); 
  }
  catch (std::runtime_error e) { 
    PyErr_SetString(PyExc_RuntimeError, e.what()); 
    return NULL; 
  }

  return Py_BuildValue("i", return_code);
}


static PyMethodDef methods[] = {
  {"_cutflow", py_cutflow, METH_VARARGS, 
   "cutflow(input_file, flags) --> cuts_list\n"
   "flags:\n"
   "\tv: verbose\n"},
  {"_make_perf_ntuple", py_perf_ntuple, METH_VARARGS, "nothings"}, 
  {NULL, NULL, 0, NULL}   /* sentinel */
};

extern "C" { 

  PyMODINIT_FUNC init_cutflow(void)
  {
    Py_InitModule("_cutflow", methods);
  }

}
