#ifndef _DISTILLER_HH
#define _DISTILLER_HH

#include <Python.h>

class RunInfo; 

static unsigned get_flags(const char* flag_str); 
static bool fill_run_info(PyObject* dict, RunInfo* info); 
static bool safe_copy(PyObject* value, std::string& dest); 
static bool safe_copy(PyObject* value, int& dest); 

#endif 
