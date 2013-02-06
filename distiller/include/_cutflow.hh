#ifndef _CUTFLOW_HH
#define _CUTFLOW_HH

#include <Python.h>

class RunInfo; 

static unsigned get_flags(const char* flag_str); 
static bool fill_run_info(PyObject* dict, RunInfo* info); 

#endif 
