#ifndef _DISTILLER_HH
#define _DISTILLER_HH

#include <Python.h>
#include "systematic_defs.hh"
#include "CutflowConfig.hh"

class RunInfo;

static unsigned get_flags(const char* flag_str);
static bool fill_run_info(PyObject* dict, RunInfo* info);
static bool info_element_copy(PyObject* value, RunInfo* info,
			      const std::string& ckey);
static bool safe_copy(PyObject* value, std::string& dest);
static bool safe_copy(PyObject* value, systematic::Systematic& dest);
static bool safe_copy(PyObject* value, CutflowType& dest);
static bool safe_copy(PyObject* value, double& dest);

#endif
