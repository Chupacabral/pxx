#ifndef PXX_TOPYOBJECT_H
#define PXX_TOPYOBJECT_H

#include <Python.h>
#include <string>
#include <vector>
#include <map>
#include <initializer_list>
#include "item.h"

namespace pxx {
  PyObject* to_pyobject(char text) {
    return PyUnicode_FromString(std::string(1, text).c_str());
  }

  PyObject* to_pyobject(const char* text) {
    return PyUnicode_FromString(text);
  }

  PyObject* to_pyobject(std::string text) {
    return PyUnicode_FromString(text.c_str());
  }

  PyObject* to_pyobject(short n) { return PyLong_FromLong(n); }
  PyObject* to_pyobject(int n) { return PyLong_FromLong(n); }
  PyObject* to_pyobject(long n) { return PyLong_FromLong(n); }
  PyObject* to_pyobject(long long n) { return PyLong_FromLongLong(n); }
  PyObject* to_pyobject(float n) { return PyFloat_FromDouble(n); }
  PyObject* to_pyobject(double n) { return PyFloat_FromDouble(n); }
  PyObject* to_pyobject(bool value) {
    return value
      ? PyBool_FromLong(true)
      : PyBool_FromLong(false);
  }

  PyObject* to_pyobject(std::vector<PyObject*> list, bool stealReferences = false) {
    PyObject* newList = PyList_New(list.size());

    // TODO: CHECK IF POINTERS WORK HERE OR IF DOUBLE *-ING.
    int index = 0;
    for (auto item = list.begin(); item < list.end(); item++, index++) {
      // Steals Item reference.
      PyList_SetItem(newList, index, *item);

      // Add reference to cancel out stolen reference, if wanted.
      if (!stealReferences) { Py_XINCREF(*item); }
    }

    return newList;
  }

  PyObject* to_pyobject(std::initializer_list<PyObject*> list, bool stealReferences = false) {
    PyObject* newList = PyList_New(list.size());

    int index = 0;
    for (auto item = list.begin(); item < list.end(); item++, index++) {
      // Steals item reference.
      PyList_SetItem(newList, index, *item);

      // Add reference to cancel out stolen reference, if wanted.
      if (!stealReferences) { Py_XINCREF(*item); }
    }

    return newList;
  }

  PyObject* to_pyobject(std::vector<Item> list, bool stealReferences = false) {
    PyObject* newList = PyList_New(list.size());

    int index = 0;
    for (auto item = list.begin(); item < list.end(); item++, index++) {
      // Steals item reference.
      PyList_SetItem(newList, index, item->to_object());

      // Add reference to cancel out stolen reference, if wanted.
      if (!stealReferences) { Py_XINCREF(item->to_object()); }
    }

    return newList;
  }

  PyObject* to_pyobject(std::initializer_list<Item> list, bool stealReferences = false) {
    PyObject* newList = PyList_New(list.size());

    int index = 0;
    for (auto item = list.begin(); item < list.end(); item++, index++) {
      // Steals item reference.
      PyList_SetItem(newList, index, item->to_object());

      // Add reference to cancel out stolen reference, if wanted.
      // Need Py_XINCREF as Item.add_reference is non-const and item is
          // const due to initializer_list.
      if (!stealReferences) { Py_XINCREF(item->to_object()); }
    }

    return newList;
  }

  // If it's not a vector of PyObjects or Items, then it shouldn't be
  // something that needs to track a PyObject reference to as it will be
  // creating new PyObjects.
  /* */
  /**
   * Creates a Python list object from the given vector.
  */
  template <typename T>
  PyObject* to_pyobject(std::vector<T> list) {
    PyObject* newList = PyList_New(list.size());

    int index = 0;
    for (auto item = list.begin(); item < list.end(); item++, index++) {
      PyList_SetItem(newList, index, to_pyobject(*item));
    }

    return newList;
  }

  // If it's not a list of PyObjects or Items, then it shouldn't be
  // something that needs to track a PyObject reference to as it will be
  // creating new PyObjects.
  /* */
  /**
   * Creates a Python list object from the given initializer list.
  */
  template <typename T>
  PyObject* to_pyobject(std::initializer_list<T> list) {
    PyObject* newList = PyList_New(list.size());

    int index = 0;
    for (auto item = list.begin(); item < list.end(); item++, index++) {
      PyList_SetItem(newList, index, to_pyobject(*item));
    }

    return newList;
  }

  /**
   * Creates a Python dictionary object from the given map.
  */
  template <typename K, typename V>
  PyObject* to_pyobject(std::map<K, V> dict) {
    PyObject* newDict = PyDict_New();

    for (auto const item : dict) {
      PyObject* firstObject = to_pyobject(item.first);
      PyObject* secondObject = to_pyobject(item.second);

      PyDict_SetItem(newDict, firstObject, secondObject);
    }

    return newDict;
  }

  PyObject* to_pyobject(PyObject* object) { return object; }

  PyObject* to_pyobject(Item object) { return object.to_object(); }
}

#endif