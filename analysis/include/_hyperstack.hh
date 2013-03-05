#ifndef HYPERSTACK_HH
#define HYPERSTACK_HH

#include <Python.h>
#include "BtagConfig.hh"
#include "systematic_defs.hh"

struct HistConfig; 

#include <string> 
#include <map>

static bool fill_config(PyObject* dict, HistConfig* config); 
static bool safe_copy(std::string key, 
		      PyObject* value, 
		      std::map<std::string, float>& fmap); 
static bool safe_copy(PyObject* value, btag::EventConfig& dest); 
static bool safe_copy(PyObject* value, syst::Systematic& dest); 


#endif 
