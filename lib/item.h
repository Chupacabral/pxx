#ifndef PXX_ITEM_H
#define PXX_ITEM_H

#include <Python.h>
#include "to_pyobject.h"
#include <vector>
#include <map>

namespace pxx {
  class Item {
    protected:
      PyObject* m_object;
    
    public:
      Item() : m_object(NULL) {}
      Item(PyObject* item) : m_object(item) {}

      template <typename T>
      Item(T item) : object(to_pyobject(item)) {}

      ~Item() {
        Py_CLEAR(m_object);
      }

      bool exists() const      { return m_object != NULL; }
      bool is_none() const     { return m_object == Py_None; }
      bool is_true() const     { return m_object == Py_True; }
      bool is_false() const    { return m_object == Py_False; }
      bool is_truthy() const   { return PyObject_IsTrue(m_object); }
      bool is_falsy() const    { return !is_truthy(); }
      bool is_boolean() const  { return is_true() || is_false(); }
      bool is_int() const      { return PyLong_Check(m_object) != 0; }
      bool is_float() const    { return PyFloat_Check(m_object) != 0; }
      bool is_list() const     { return PyList_Check(m_object) != 0; }
      bool is_dict() const     { return PyDict_Check(m_object) != 0; }
      bool is_set() const      { return PySet_Check(m_object) != 0; }
      bool is_bytes() const    { return PyList_Check(m_object) != 0; }
      bool is_string() const   { return PyUnicode_Check(m_object) != 0; }
      bool is_callable() const { return PyCallable_Check(m_object) != 0; }
      bool is_iterable() const { return PyIter_Check(m_object) != 0; }


      bool is_type(PyTypeObject* type) const {
        return PyObject_TypeCheck(m_object, type) != 0;
      }

      bool is_instance_of(PyObject* cls) const {
        return PyObject_IsInstance(m_object, cls);
      }

      bool is_subclass(PyObject* cls) const {
        return PyObject_IsSubclass(m_object, cls);
      }

      template <typename T>
      Item get_attr(T attrName) {
        PyObject* attr = PyObject_GetAttr(m_object, to_pyobject(attrName));


      }

      bool operator<(const Item other) const {
        PyObject* otherObject = other.to_object();

        return &m_object < &otherObject;
      }

      std::string to_string() const {
        if (m_object == NULL) { return nullptr; }

        PyObject* obString = PyObject_Str(m_object);
        PyObject* obUTF = PyUnicode_AsEncodedString(obString, "UTF-8", "~E~");

        return PyBytes_AsString(obUTF);
      }

      const char* to_cstring() const {
        if (m_object == NULL) { return NULL; }

        PyObject* obString = PyObject_Str(m_object);
        PyObject* obUTF = PyUnicode_AsEncodedString(obString, "UTF-8", "~E~");

        return PyBytes_AsString(obUTF);
      }

      int to_int() const {
        return PyLong_AsLong(m_object);
      }

      long to_long() const {
        return this->to_int();
      }
      
      long long to_longlong() const {
        return PyLong_AsLongLong(m_object);
      }
      
      unsigned long long to_ulonglong() const {
        return PyLong_AsUnsignedLongLong(m_object);
      }

      double to_double() const {
        return PyFloat_AsDouble(m_object);
      }

      PyObject* to_object() const { return m_object; }

      std::vector<Item> to_vector() const {
        return std::vector<Item>();
      }

      std::map<Item, Item> to_map() const {
        return std::map<Item, Item>();
      }

      void operator=(PyObject* item) { m_object = item; }
  };
}

#endif