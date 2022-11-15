#ifndef PXX_LIB_H
#define PXX_LIB_H

// This file is for the primary PXX library functions.
#include "pxx_py.h"

#define PY_SSIZE_T_CLEAN
#include <Python.h>
#include <string>
#include <vector>
#include <map>
#include <initializer_list>
#include <utility>

namespace pxx {
  void InitializeInterpreter(int initsigs = -1) {
      if (initsigs != -1) {
        Py_InitializeEx(initsigs);
      }
      else {
        Py_Initialize();
      }
  }

  void InitializeInterpreter(PyConfig* config) {
    Py_InitializeFromConfig(config);
  }

  void LoadDirectoryInPython() {  
    PyRun_SimpleString("import sys");
    PyRun_SimpleString("sys.path.append(\".\")");
  }

  bool FinalizeInterpreter() {
    return Py_FinalizeEx() == 0;
  }

  /**
   * Imports and return the module matching the specified name from the given
   * string.
   * 
   * @param moduleName The name of the module to import.
   * @return The PyObject for the imported module, or NULL on failure.
  */
  py::Module LoadModule(std::string moduleName) {
    const char* modName = moduleName.c_str();
    PyObject *module = PyImport_ImportModule(modName);

    return module;
  }

  /**
   * Gets the attribute with the name specified in the given string.
   * 
   * @param ob The PyObject to get the attribute from.
   * @param attrName The name of the attribute on the object to get.
   * @return The PyObject for the attribute, or NULL on failure.
  */
  py::Any GetAttribute(py::Any ob, std::string attrName) {
    return PyObject_GetAttrString(ob, attrName.c_str());
  }

  /**
   * Gets the attribute with the name specified in the given string from the
   * specified module.
   * 
   * @param module The name of the module to get the attribute from.
   * @param attrName The name of the attribute on the object to get.
   * @return The PyObject for the attribute, or NULL on failure.
  */
  py::Any GetAttribute(std::string module, std::string attrName) {
    PyObject* mod = LoadModule(module);
    return PyObject_GetAttrString(mod, attrName.c_str());
  }

  /**
   * Gets the attribute with the name specified in the given string from the
   * specified module. The attribute is represented as a list of attributes
   * to go through, starting from the module.
   * 
   * @param module The name of the module to get the attribute from.
   * @param attrName The list of attributes to go through.
   * @return The PyObject for the attribute, or NULL on failure.
  */
  py::Any GetAttribute(
    std::string module,
    const std::initializer_list<std::string> &attrs
  ) {
    PyObject* currentAttr = LoadModule(module);

    for (auto attr : attrs) {
      currentAttr = GetAttribute(currentAttr, attr);

      if (currentAttr == NULL) { break; }
    }

    return currentAttr;
  }

  /**
   * Gets the attribute with the name specified in the given string from the
   * specified object. The attribute is represented as a list of attributes
   * to go through, starting from the object.
   * 
   * @param module The PyObject to get the attribute from.
   * @param attrName The list of attributes to go through.
   * @return The PyObject for the attribute, or NULL on failure.
  */
  py::Any GetAttribute(
    py::Any ob,
    const std::initializer_list<std::string> &attrs
  ) {
    PyObject* currentAttr = ob;

    for (auto attr : attrs) {
      currentAttr = GetAttribute(currentAttr, attr);

      if (currentAttr == NULL) { break; }
    }

    return currentAttr;
  }

  // PyObject* CallFunction(
  //   PyObject* fn,
  //   const std::initializer_list<PyObject*> args
  // );

  py::String MakeString(std::string text) {
    return PyUnicode_FromString(text.c_str());
  }

  py::String MakeString(const char* text) {
    return PyUnicode_FromString(text);
  }

  py::List MakeList(std::vector<py::Any> list) {
    PyObject* pyList = PyList_New(list.size());

    for (int x = 0; x < list.size(); x++) {
      PyList_Insert(pyList, x, list.at(x));
    }

    return pyList;
  }

  py::List MakeList(std::initializer_list<py::Any> list) {
    size_t index = 0;
    std::vector<PyObject*> newList;

    for (const auto item : list) {
      newList.push_back(item);
    }

    return MakeList(newList);
  }

  py::Tuple MakeTuple(std::vector<py::Any> list) {
    PyObject* pyList = PyTuple_New(list.size());

    for (int x = 0; x < list.size(); x++) {
      PyTuple_SetItem(pyList, x, list.at(x));
    }

    return pyList;
  }

  py::Tuple MakeTuple(std::initializer_list<py::Any> list) {
    size_t index = 0;
    std::vector<PyObject*> newList;

    for (const auto item : list) {
      newList.push_back(item);
    }

    return MakeTuple(newList);
  }

  // TODO: Convert T to pxx::py::Any?
  template <typename T>
  py::Dictionary MakeDict(std::map<std::string, T> dict) {
    PyObject* pyDict = PyDict_New();

    for (auto const& item : dict) {
      PyDict_SetItemString(pyDict, item.first.c_str(), item.second);
    }

    return pyDict;
  }

  py::Dictionary MakeDict(std::map<py::Any, py::Any> dict) {
    PyObject* pyDict = PyDict_New();

    for (auto const& item : dict) {
      PyDict_SetItem(pyDict, item.first, item.second);
    }

    return pyDict;
  }

  py::Integer MakeInt(long long n) {
    return PyLong_FromLongLong(n);
  }

  py::Integer MakeInt(std::string n) {
    return PyLong_FromString(n.c_str(), NULL, 10);
  }

  py::Integer MakeInt(const char* n) {
    return PyLong_FromString(n, NULL, 10);
  }

  std::string ToString(py::Any ob) {
    if (ob == NULL) { return NULL; }
  
    PyObject* obStr = PyObject_Str(ob);
    PyObject* obUTF = PyUnicode_AsEncodedString(obStr, "UTF-8", "~E~");

    return PyBytes_AsString(obUTF);
  }

  const char* ToCString(py::Any ob) {
    if (ob == NULL) { return NULL; }
    
    PyObject* obStr = PyObject_Str(ob);
    PyObject* obUTF = PyUnicode_AsEncodedString(obStr, "UTF-8", "~E~");

    return PyBytes_AsString(obUTF);
  }

  int* ToInt(py::Any ob) {
    int converted = PyLong_AsLong(ob);

    py::Error error = PyErr_Occurred();

    return error
      ? nullptr
      : &converted;
  }

  long* ToLong(py::Any ob) {
    long converted = PyLong_AsLong(ob);

    py::Error error = PyErr_Occurred();

    return error
      ? nullptr
      : &converted;
  }

  long long* ToLongLong(py::Any ob) {
    long long converted = PyLong_AsLongLong(ob);

    py::Error error = PyErr_Occurred();

    return error
      ? nullptr
      : &converted;
  }

  unsigned int* ToUInt(py::Integer ob) {
    unsigned int converted = PyLong_AsUnsignedLong(ob);

    py::Error error = PyErr_Occurred();

    return error
      ? nullptr
      : &converted;
  }

  unsigned long* ToULong(py::Integer ob) {
    unsigned long converted = PyLong_AsUnsignedLong(ob);

    py::Error error = PyErr_Occurred();

    return error
      ? nullptr
      : &converted;
  }

  unsigned long long* ToULongLong(py::Integer ob) {
    unsigned long long converted = PyLong_AsUnsignedLongLong(ob);

    py::Error error = PyErr_Occurred();

    return error
      ? nullptr
      : &converted;
  }
  
  std::vector<py::Any> ListToVector(py::List ob) {
    if (ob == NULL) { return std::vector<PyObject*>(); }

    std::vector<PyObject*> list;

    for (int x = 0; x < PyList_Size(ob); x++) {
      list.push_back(PyList_GetItem(ob, x));
    }

    return list;
  }

  bool IsNone(py::Any ob)     { return ob == Py_None; }
  bool IsTrue(py::Any ob)     { return ob == Py_True; }
  bool IsFalse(py::Any ob)    { return ob == Py_False; }
  bool IsTruthy(py::Any ob)   { return PyObject_IsTrue(ob); }
  bool IsFalsy(py::Any ob)    { return !IsTruthy(ob); }
  bool IsBoolean(py::Any ob)  { return IsTrue(ob) || IsFalse(ob); }
  bool IsInteger(py::Any ob)  { return PyLong_Check(ob) != 0; }
  bool IsFloat(py::Any ob)    { return PyFloat_Check(ob) != 0; }
  bool IsList(py::Any ob)     { return PyList_Check(ob) != 0; }
  bool IsDict(py::Any ob)     { return PyDict_Check(ob) != 0; }
  bool IsSet(py::Any ob)      { return PySet_Check(ob) != 0; }
  bool IsBytes(py::Any ob)    { return PyList_Check(ob) != 0; }
  bool IsString(py::Any ob)   { return PyUnicode_Check(ob) != 0; }
  bool IsCallable(py::Any ob) { return PyCallable_Check(ob) != 0; }
  bool IsIterable(py::Any ob) { return PyIter_Check(ob) != 0; }

  bool IsType(py::Any ob, PyTypeObject* type) {
    return PyObject_TypeCheck(ob, type) != 0;
  }

  bool IsInstanceOf(py::Any ob, py::Type cls) {
    return PyObject_IsInstance(ob, cls);
  }

  bool IsSubclass(py::Any ob, py::Type cls) {
    return PyObject_IsSubclass(ob, cls);
  }

  py::Any CallFunction(py::Function fn, py::Tuple args) {
    return PyObject_CallObject(fn, args);
  }

  py::Any CallFunction(
    py::Function fn,
    std::initializer_list<py::Any> args
  ) {
    return PyObject_CallObject(fn, MakeTuple(args));
  }

  py::Any CallFunction(
    py::Function fn
  ) {
    return PyObject_CallNoArgs(fn);
  }
}

#endif