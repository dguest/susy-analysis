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
  RunInfo info;

  bool ok = PyArg_ParseTuple
    (args,"OO&|s:distiller", &py_input_files, fill_run_info, &info,
     &flags_str);
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
  if (info.systematic != systematic::NONE) flags |= cutflag::disable_sf_err;

  typedef std::vector<std::pair<std::string, double> > CCOut;
  CCOut pass_numbers;
  try {
    StopDistiller distiller(input_files, info, flags);
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
    PyObject* tuple = Py_BuildValue("sf", itr->first.c_str(), itr->second);
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
  if (strchr(flags_str,'c')) flags |= cutflag::cutflow;
  if (strchr(flags_str,'d')) {
    flags |= cutflag::is_data;
  }
  else {
    flags |= cutflag::truth;
  }
  if (strchr(flags_str,'e')) flags |= cutflag::save_all_events;
  if (strchr(flags_str,'f')) flags |= cutflag::is_atlfast;
  if (strchr(flags_str,'g')) flags |= cutflag::get_branches;
  if (strchr(flags_str,'i')) flags |= cutflag::spartid;
  // if (strchr(flags_str,'j')) flags |= cutflag::vector_output;
  // if (strchr(flags_str,'m')) flags |= cutflag::mv3;
  if (strchr(flags_str,'p')) flags |= cutflag::boson_pt_reweight;
  // if (strchr(flags_str,'r')) flags |= cutflag::save_ratios;
  if (strchr(flags_str,'t')) flags |= cutflag::all_trig_branches;
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
    const std::string ckey = PyString_AsString(key);
    if (! info_element_copy(value, info, ckey) ) return false;
  }
  return true;
}


static bool info_element_copy(PyObject* value, RunInfo* info,
			      const std::string& ckey) {

#define TRY_VAL(key)				\
  do {						\
    if (ckey == #key) {					\
      if (!safe_copy(value, info->key)) return false;	\
      return true;					\
    }							\
  } while (false)

  TRY_VAL(grl);
  TRY_VAL(btag_cal_dir);
  TRY_VAL(btag_cal_file);
  TRY_VAL(systematic);
  TRY_VAL(cutflow_type);
  TRY_VAL(boson_pt_max_mev);
  TRY_VAL(truth_met_max_mev);
  TRY_VAL(pu_config);
  TRY_VAL(pu_lumicalc);
  TRY_VAL(out_ntuple);
  TRY_VAL(mumet_out_ntuple);
  TRY_VAL(leptmet_out_ntuple);
  TRY_VAL(eljet_out_ntuple);
  TRY_VAL(eljet_mumet_out_ntuple);

  std::string err = "got unknown string in distiller info dict: " + ckey;
  PyErr_SetString(PyExc_ValueError, err.c_str());
  return false;

#undef TRY_VAL
}

static bool safe_copy(PyObject* value, std::string& dest) {
  char* str = PyString_AsString(value);
  if (PyErr_Occurred()) return false;
  dest = str;
  return true;
}

#define TRY_VAL(value)				\
  do {					\
    if (name == #value) {			\
      dest = value;				\
      return true;				\
    }						\
  } while (false)

// black magic!
#define TRY_UD(value) TRY_VAL(value ## UP); TRY_VAL(value ## DOWN)

static bool safe_copy(PyObject* value, systematic::Systematic& dest) {
  char* sysname = PyString_AsString(value);
  if (PyErr_Occurred()) return false;
  std::string name(sysname);
  using namespace systematic;

  TRY_VAL(NONE);
  TRY_VAL(JESUP);
  TRY_VAL(JESDOWN);
  TRY_VAL(JER);
  TRY_VAL(METUP);
  TRY_VAL(METDOWN);
  TRY_VAL(METRES);

  TRY_UD(EGZEE);
  TRY_UD(EGMAT);
  TRY_UD(EGPS);
  TRY_UD(EGLOW);
  TRY_UD(EGRES);
  TRY_UD(MMS);
  TRY_UD(MID);
  TRY_UD(MSCALE);

  std::string problem = "got undefined systematic: " + name;
  PyErr_SetString(PyExc_ValueError,problem.c_str());
  return false;

}
#undef TRY_VAL
#undef TRY_UD

#define TRY_VAL(NAMESPACE, value)		\
  do {					\
    if (name == #value) {			\
      dest = NAMESPACE::value;			\
      return true;				\
    }						\
  } while (false)


static bool safe_copy(PyObject* value, CutflowType& dest) {
  char* sysname = PyString_AsString(value);
  if (PyErr_Occurred()) return false;
  std::string name(sysname);

  TRY_VAL(CutflowType, SIGNAL);
  TRY_VAL(CutflowType, NONE);
  TRY_VAL(CutflowType, CRZ);
  TRY_VAL(CutflowType, CRW);
  TRY_VAL(CutflowType, CRT);

  std::string problem = "got undefined cutflow type: " + name;
  PyErr_SetString(PyExc_ValueError,problem.c_str());
  return false;
}
#undef TRY_VAL


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
