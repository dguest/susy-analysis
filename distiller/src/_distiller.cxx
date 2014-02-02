// python wrapper for susy cutflow / d3pd reader
// Author: Daniel Guest (dguest@cern.ch)
#include <Python.h>
#include <string> 
#include <vector>
#include <map>
#include <stdexcept>

#include "_distiller.hh"
#include "StopDistiller.hh"

#include "cutflag.hh"
#include "RunInfo.hh"
#include "systematic_defs.hh"
#include "CutflowConfig.hh"


static PyObject* py_distiller(PyObject *self, 
			      PyObject *args)
{
  PyObject* py_input_files; 
  const char* flags_str = ""; 
  const char* out_ntuple = ""; 
  RunInfo info; 

  bool ok = PyArg_ParseTuple
    (args,"OO&|ss:distiller", &py_input_files, fill_run_info, &info, 
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
  
  unsigned flags = get_flags(flags_str); 
  assert( (flags & (cutflag::jetfitter_charm | cutflag::mv3)) == 0); 

  typedef std::vector<std::pair<std::string, int> > CCOut; 
  CCOut pass_numbers; 
  try { 
    StopDistiller distiller(input_files, info, flags, out_ntuple); 
    pass_numbers = distiller.run_cutflow(); 
  }
  catch (std::runtime_error& e) { 
    PyErr_SetString(PyExc_RuntimeError, e.what()); 
    return NULL; 
  }
  catch (std::exception& e) { 
    PyErr_SetString(PyExc_Exception, e.what()); 
    return NULL; 
  }

  PyObject* out_list = PyList_New(0); 
  for (auto itr = pass_numbers.begin(); 
       itr != pass_numbers.end(); 
       itr++){ 
    PyObject* tuple = Py_BuildValue("si", itr->first.c_str(), itr->second);
    if(PyList_Append(out_list,tuple)) { 
      return NULL; 
    }
  }
  
  return out_list; 

}

static unsigned get_flags(const char* flags_str) 
{
  unsigned flags = 0; 
  // flag 'a' is reserved for 'aggressive'
  if (strchr(flags_str,'b')) flags |= cutflag::debug_susy; 
  if (strchr(flags_str,'c')) flags |= cutflag::jetfitter_charm; 
  if (strchr(flags_str,'d')) {
    flags |= cutflag::is_data; 
  }
  else { 
    flags |= cutflag::truth; 
  }
  if (strchr(flags_str,'e')) flags |= cutflag::save_all_events; 
  if (strchr(flags_str,'f')) flags |= cutflag::is_atlfast; 
  if (strchr(flags_str,'g')) flags |= cutflag::get_branches; 
  if (strchr(flags_str,'h')) flags |= cutflag::old_skim; 
  if (strchr(flags_str,'i')) flags |= cutflag::spartid; 
  // if (strchr(flags_str,'j')) flags |= cutflag::vector_output; 
  if (strchr(flags_str,'m')) flags |= cutflag::mv3; 
  if (strchr(flags_str,'p')) flags |= cutflag::boson_pt_reweight; 
  // if (strchr(flags_str,'r')) flags |= cutflag::save_ratios; 
  if (strchr(flags_str,'u')) flags |= cutflag::generate_pileup; 
  if (strchr(flags_str,'v')) flags |= cutflag::verbose; 
  if (strchr(flags_str,'z')) flags |= cutflag::maximum_compression; 
  return flags; 
}

static bool fill_run_info(PyObject* dict, RunInfo* info) { 
  if (!dict) return false; 
  if (!PyDict_Check(dict)) return false; 
  PyObject* key; 
  PyObject* value; 
  Py_ssize_t pos = 0; 
  while(PyDict_Next(dict, &pos, &key, &value)) { 
    if (!PyString_Check(key) || PyString_Size(key) == 0) { 
      PyErr_SetString(PyExc_ValueError, 
		      "distiller info dict includes non-string key"); 
      return false; 
    }
    std::string ckey = PyString_AsString(key); 
    if (ckey == "grl") { 
      if (!safe_copy(value, info->grl)) return false; 
    }
    else if (ckey == "trigger") { 
      if (!safe_copy(value, info->trigger)) return false; 
    }
    else if (ckey == "btag_cal_dir") { 
      if (!safe_copy(value, info->btag_cal_dir)) return false; 
    }
    else if (ckey == "btag_cal_file") { 
      if (!safe_copy(value, info->btag_cal_file)) return false;
    }
    else if (ckey == "systematic") { 
      if (!safe_copy(value, info->systematic)) return false; 
    }
    else if (ckey == "cutflow_type") { 
      if (!safe_copy(value, info->cutflow_type)) return false; 
    }
    else if (ckey == "boson_pt_max_mev") { 
      if (!safe_copy(value, info->boson_pt_max_mev)) return false; 
    }
    else if (ckey == "truth_met_max_mev") { 
      if (!safe_copy(value, info->truth_met_max_mev)) return false; 
    }
    else if (ckey == "pu_config") { 
      if (!safe_copy(value, info->pu_config)) return false; 
    }
    else if (ckey == "pu_lumicalc") { 
      if (!safe_copy(value, info->pu_lumicalc)) return false; 
    }
    else { 
      std::string err = "got unknown string in distiller info dict: " + ckey; 
      PyErr_SetString(PyExc_ValueError, err.c_str()); 
      return false; 
    }
  }
  return true; 
}

static bool safe_copy(PyObject* value, std::string& dest) { 
  char* str = PyString_AsString(value); 
  if (PyErr_Occurred()) return false; 
  dest = str; 
  return true; 
}
static bool safe_copy(PyObject* value, systematic::Systematic& dest) { 
  char* sysname = PyString_AsString(value); 
  if (PyErr_Occurred()) return false; 
  std::string name(sysname); 
  using namespace systematic; 
  if (name == "JESUP") { 
    dest = JESUP; 
    return true; 
  }
  else if (name == "JESDOWN") { 
    dest = JESDOWN; 
    return true; 
  }
  else if (name == "JER") { 
    dest = JER; 
    return true; 
  }
  else if (name == "NONE") { 
    dest = NONE; 
    return true; 
  }
  else { 
    std::string problem = "got undefined systematic: " + name; 
    PyErr_SetString(PyExc_ValueError,problem.c_str()); 
    return false; 
  }
}

static bool safe_copy(PyObject* value, CutflowType& dest) { 
  char* sysname = PyString_AsString(value); 
  if (PyErr_Occurred()) return false; 
  std::string name(sysname); 
  if (name == "NOMINAL") { 
    dest = CutflowType::NOMINAL; 
    return true; 
  }
  else if (name == "NONE") { 
    dest = CutflowType::NONE; 
    return true; 
  }
  else if (name == "ELECTRON_CR") { 
    dest = CutflowType::ELECTRON_CR; 
    return true; 
  }
  else if (name == "MUON_CR") { 
    dest = CutflowType::MUON_CR; 
    return true; 
  }
  else { 
    std::string problem = "got undefined cutflow type: " + name; 
    PyErr_SetString(PyExc_ValueError,problem.c_str()); 
    return false; 
  }
}

static bool safe_copy(PyObject* value, double& dest) { 
  double the_double = PyFloat_AsDouble(value); 
  if (PyErr_Occurred()) return false; 
  dest = the_double; 
  return true; 
}


static PyMethodDef methods[] = {
  {"_distiller", py_distiller, METH_VARARGS, 
   "cutflow(input_file, flags) --> cuts_list\n"
   "flags:\n"
   "\tv: verbose\n"},
  {NULL, NULL, 0, NULL}   /* sentinel */
};

extern "C" { 

  PyMODINIT_FUNC init_distiller(void)
  {
    Py_InitModule("_distiller", methods);
  }

}
