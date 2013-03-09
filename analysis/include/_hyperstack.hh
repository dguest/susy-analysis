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

static bool safe_copy(PyObject* value, btag::JetTag& dest); 
static bool safe_copy(PyObject* value, syst::Systematic& dest); 
static bool safe_copy(PyObject* value, std::string& dest); 


#endif 
