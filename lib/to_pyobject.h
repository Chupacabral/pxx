#ifndef PXX_BASE_H
#define PXX_BASE_H

#include "py_types.h"
#include "item.h"
#include <string>
#include <vector>
#include <map>
#include <initializer_list>

namespace pxx {
  py::String to_pyobject(const char* text) {
    return PyUnicode_FromString(text);
  }

  py::String to_pyobject(std::string text) {
    return PyUnicode_FromString(text.c_str());
  }

  py::Int to_pyobject(short n) { return PyLong_FromLong(n); }
  py::Int to_pyobject(int n) { return PyLong_FromLong(n); }
  py::Int to_pyobject(long n) { return PyLong_FromLong(n); }
  py::Int to_pyobject(long long n) { return PyLong_FromLongLong(n); }
  py::Float to_pyobject(float n) { return PyFloat_FromDouble(n); }
  py::Float to_pyobject(double n) { return PyFloat_FromDouble(n); }
  py::Boolean to_pyobject(bool value) {
    return value
      ? PyBool_FromLong(true)
      : PyBool_FromLong(false);
  }

  /**
   * Creates a Python list object from the given vector.
  */
  template <typename T>
  py::List to_pyobject(std::vector<T> list) {
    py::List newList = PyList_New(0);

    for (const auto item : list) {
      py::Any itemObject = to_pyobject(item);

      PyList_Append(newList, itemObject);
    }

    return newList;
  }

  /**
   * Creates a Python list object from the given initializer list.
  */
  template <typename T>
  py::List to_pyobject(std::initializer_list<T> list) {
    py::List newList = PyList_New(0);

    for (const auto item : list) {
      py::Any itemObject = to_pyobject(item);

      PyList_Append(newList, itemObject);
    }

    return newList;
  }

  /**
   * Creates a Python dictionary object from the given map.
  */
  template <typename K, typename V>
  py::Dict to_pyobject(std::map<K, V> dict) {
    py::Dict newDict = PyDict_New();

    for (auto const item : dict) {
      py::Any firstObject = to_pyobject(item.first);
      py::Any secondObject = to_pyobject(item.second);

      PyDict_SetItem(newDict, firstObject, secondObject);
    }

    return newDict;
  }

  py::Any to_pyobject(py::Any object) { return object; }

  py::Any to_pyobject(Item object) { return object.to_object(); }
}

#endif