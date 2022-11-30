#ifndef PXX_TOPYOBJECT_H
#define PXX_TOPYOBJECT_H

#include <Python.h>
#include <string>
#include <vector>
#include <map>
#include <initializer_list>
#include "item.h"

namespace pxx {
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

  /**
   * Creates a Python list object from the given vector.
  */
  template <typename T>
  PyObject* to_pyobject(std::vector<T> list) {
    PyObject* newList = PyList_New(0);

    for (const auto item : list) {
      PyObject* itemObject = to_pyobject(item);

      PyList_Append(newList, itemObject);
    }

    return newList;
  }

  /**
   * Creates a Python list object from the given initializer list.
  */
  template <typename T>
  PyObject* to_pyobject(std::initializer_list<T> list) {
    PyObject* newList = PyList_New(0);

    for (const auto item : list) {
      PyObject* itemObject = to_pyobject(item);

      PyList_Append(newList, itemObject);
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