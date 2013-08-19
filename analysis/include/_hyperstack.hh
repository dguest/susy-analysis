#ifndef HYPERSTACK_HH
#define HYPERSTACK_HH

#include <Python.h>
#include "BtagConfig.hh"
#include "systematic_defs.hh"
#include "RegionConfig.hh"
#include "typedefs.hh"

// struct HistConfig; 

#include <string> 
#include <map>
#include <vector> 


template <typename T>
static bool require(PyObject* dict, std::string key, T& dest); 
template <typename T>
static bool copy(PyObject* dict, std::string key, T& dest); 

template <typename T>
static bool safe_copy(PyObject* list, std::vector<T>* dest); 
template <typename T>
static bool safe_copy(PyObject* list, std::vector<T>& dest); 

static bool safe_copy(PyObject* dict, RegionConfig& region); 
static bool safe_copy(PyObject* value, std::string& dest); 
static bool safe_copy(PyObject* value, ull_t& dest); 
static bool safe_copy(PyObject* value, unsigned& dest); 
static bool safe_copy(PyObject* value, double& dest); 
static bool safe_copy(PyObject* value, btag::OperatingPoint& dest); 
static bool safe_copy(PyObject* value, syst::Systematic& dest); 
static bool safe_copy(PyObject* value, reg::Type& dest); 
static bool safe_copy(PyObject* value, reg::Hists& dest); 


// --- implementation ---

template <typename T>
static bool safe_copy(PyObject* list, std::vector<T>* dest) { 
  if (!dest) { 
    PyErr_SetString(PyExc_ValueError, "tried to copy to a null pointer"); 
    return false; 
  }
  return safe_copy(list, *dest); 
}

template<typename T>
static bool safe_copy(PyObject* list, std::vector<T>& dest) { 
  const int n_values = PyList_Size(list); 
  if (PyErr_Occurred()) return NULL; 
  for (int val_n = 0; val_n < n_values; val_n++) { 
    PyObject* entry = PyList_GetItem(list, val_n); 
    T object;
    if (!safe_copy(entry, object)) return false; 
    dest.push_back(object); 
  }
  return true; 
}


template<typename T>
static bool require(PyObject* dict, std::string key, T& dest) {
  PyObject* string = PyDict_GetItemString(dict, key.c_str()); 
  if (!string) { 
    std::string prob = "regions must have specified '" + key + "'"; 
    PyErr_SetString(PyExc_ValueError,prob.c_str()); 
    return false; 
  }
  if (!safe_copy(string, dest)) return false; 
  return true; 
}

template<typename T>
static bool copy(PyObject* dict, std::string key, T& dest) {
  PyObject* string = PyDict_GetItemString(dict, key.c_str()); 
  if (!string) return true; 	// it's not required
  if (!safe_copy(string, dest)) return false; 
  return true; 
}

#endif 
