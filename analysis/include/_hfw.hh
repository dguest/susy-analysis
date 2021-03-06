#ifndef HYPERSTACK_HH
#define HYPERSTACK_HH

#include <Python.h>
#include "btag_enums.hh"
#include "systematic_defs.hh"
#include "RegionConfig.hh"
#include "typedefs.hh"

#include <string>
#include <map>
#include <vector>
#include <utility>

template <typename T>
static bool require(PyObject* dict, std::string key, T& dest);
template <typename T>
static bool copy(PyObject* dict, std::string key, T& dest);

template <typename T>
static bool safe_copy(PyObject* list, std::vector<T>* dest);
template <typename T>
static bool safe_copy(PyObject* list, std::vector<T>& dest);
template <typename T, typename U>
static bool safe_copy(PyObject* tup, std::pair<T, U>& dest);

static bool safe_copy(PyObject* dict, RegionConfig& region);
static bool safe_copy(PyObject* value, std::string& dest);
static bool safe_copy(PyObject* value, double& dest);
static bool safe_copy(PyObject* value, long& dest);
static bool safe_copy(PyObject* value, bool& dest);
static bool safe_copy(PyObject* value, syst::Systematic& dest);
static bool safe_copy(PyObject* value, reg::Type& dest);
static bool safe_copy(PyObject* value, reg::Hists& dest);
static bool safe_copy(PyObject* value, reg::BosonPtCorrection& dest);
static bool safe_copy(PyObject* value, reg::Selection& dest);
static bool safe_copy(PyObject* value, reg::Stream& dest);

// --- implementation of sequence -> atomic copies ---

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
  if (!PyList_Check(list)) {
    PyErr_SetString(PyExc_ValueError, "python object must be list");
    return false;
  }
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

template<typename T, typename U>
static bool safe_copy(PyObject* tup, std::pair<T, U>& dest) {
  if (!PyTuple_Check(tup)) {
    PyErr_SetString(PyExc_ValueError, "object is not tupple");
    return false;
  }
  if (PyTuple_Size(tup) != 2){
    PyErr_SetString(PyExc_IndexError, "tuple->pair must be size two");
    return false;
  }
  T item1;
  if (!safe_copy(PyTuple_GetItem(tup, 0), item1)) return false;
  U item2;
  if (!safe_copy(PyTuple_GetItem(tup, 1), item2)) return false;
  dest.first = item1;
  dest.second = item2;
  return true;
}

// -------- top level routines -------------

namespace {
  bool add_key_to_exception(const std::string&);
}

template<typename T>
static bool require(PyObject* dict, std::string key, T& dest) {
  PyObject* string = PyDict_GetItemString(dict, key.c_str());
  if (!string) {
    std::string prob = "regions must have specified '" + key + "'";
    PyErr_SetString(PyExc_ValueError,prob.c_str());
    return false;
  }
  if (!safe_copy(string, dest)) return add_key_to_exception(key);
  return true;
}

template<typename T>
static bool copy(PyObject* dict, std::string key, T& dest) {
  PyObject* string = PyDict_GetItemString(dict, key.c_str());
  if (!string) return true; 	// it's not required
  if (!safe_copy(string, dest)) return add_key_to_exception(key);
  return true;
}

namespace {
  bool add_key_to_exception(const std::string& key) {
    PyObject *type, *value, *traceback;
    PyErr_Fetch(&type, &value, &traceback);
    std::string error = PyUnicode_AsUTF8(value);
    error += " (while copying '" + key + "')";
    PyErr_SetString(type, error.c_str());
    return false;
  }
}

#endif
