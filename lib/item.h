#ifndef PXX_ITEM_H
#define PXX_ITEM_H

#include "py_types.h"
#include "to_pyobject.h"
#include <vector>
#include <map>

namespace pxx {
  class Item {
    protected:
      py::Any object;
    
    public:
      Item() : object(NULL) {}
      Item(py::Any item) : object(item) {}

      template <typename T>
      Item(T item) : object(to_pyobject(item)) {}

      bool exists() const      { return this->object != NULL; }
      bool is_none() const     { return this->object == Py_None; }
      bool is_true() const     { return this->object == Py_True; }
      bool is_false() const    { return this->object == Py_False; }
      bool is_truthy() const   { return PyObject_IsTrue(this->object); }
      bool is_falsy() const    { return !is_truthy(); }
      bool is_boolean() const  { return is_true() || is_false(); }
      bool is_int() const      { return PyLong_Check(this->object) != 0; }
      bool is_float() const    { return PyFloat_Check(this->object) != 0; }
      bool is_list() const     { return PyList_Check(this->object) != 0; }
      bool is_dict() const     { return PyDict_Check(this->object) != 0; }
      bool is_set() const      { return PySet_Check(this->object) != 0; }
      bool is_bytes() const    { return PyList_Check(this->object) != 0; }
      bool is_string() const   { return PyUnicode_Check(this->object) != 0; }
      bool is_callable() const { return PyCallable_Check(this->object) != 0; }
      bool is_iterable() const { return PyIter_Check(this->object) != 0; }


      bool is_type(PyTypeObject* type) const {
        return PyObject_TypeCheck(this->object, type) != 0;
      }

      bool is_instance_of(py::Type cls) const {
        return PyObject_IsInstance(this->object, cls);
      }

      bool is_subclass(py::Type cls) const {
        return PyObject_IsSubclass(this->object, cls);
      }

      bool operator<(const Item other) const {
        py::Unknown otherObject = other.to_object();

        return &this->object < &otherObject;
      }

      std::string to_string() const {
        if (this->object == NULL) { return nullptr; }

        py::String obString = PyObject_Str(this->object);
        py::String obUTF = PyUnicode_AsEncodedString(obString, "UTF-8", "~E~");

        return PyBytes_AsString(obUTF);
      }

      const char* to_cstring() const {
        if (this->object == NULL) { return NULL; }

        py::String obString = PyObject_Str(this->object);
        py::String obUTF = PyUnicode_AsEncodedString(obString, "UTF-8", "~E~");

        return PyBytes_AsString(obUTF);
      }

      int to_int() const {
        return PyLong_AsLong(this->object);
      }

      long to_long() const {
        return this->to_int();
      }
      
      long long to_longlong() const {
        return PyLong_AsLongLong(this->object);
      }
      
      unsigned long long to_ulonglong() const {
        return PyLong_AsUnsignedLongLong(this->object);
      }

      double to_double() const {
        return PyFloat_AsDouble(this->object);
      }

      py::Any to_object() const { return this->object; }

      std::vector<Item> to_vector() const {
        return std::vector<Item>();
      }

      std::map<Item, Item> to_map() const {
        return std::map<Item, Item>();
      }

      void operator=(py::Any item) { this->object = item; }
  };
}

#endif