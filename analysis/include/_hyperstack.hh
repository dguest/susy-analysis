#ifndef HYPERSTACK_HH
#define HYPERSTACK_HH

#include <Python.h>
#include "BtagConfig.hh"
#include "systematic_defs.hh"
#include "RegionConfig.hh"

struct HistConfig; 

#include <string> 
#include <map>
#include <vector> 

static bool fill_regions(PyObject* dict, std::vector<RegionConfig>* regions); 
static bool fill_region(PyObject* dict, RegionConfig* region); 
static bool fill_jettag(PyObject* list, std::vector<btag::JetTag>*); 
static bool fill_config(PyObject* dict, HistConfig* config); 
static bool safe_copy(std::string key, 
		      PyObject* value, 
		      std::map<std::string, float>& fmap); 
// kill me
static bool safe_copy(PyObject* value, btag::EventConfig& dest); 
static bool safe_copy(PyObject* value, btag::JetTag& dest); 
static bool safe_copy(PyObject* value, syst::Systematic& dest); 


#endif 
