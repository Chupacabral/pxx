#ifndef PXX_ITEM_H
#define PXX_ITEM_H

#include <Python.h>
#include <iostream>
#include <string>
// #include <vector>

namespace pxx {
  class Boolean;
  class Dict;
  class Float;
  class Int;
  class List;
  class String;
}

namespace pxx {
  class Item {
    protected:
      PyObject* m_object;

      void __remove_object__() { m_object = NULL; }
    
    public:
      /// @brief Creates a new Item with NULL as the Python object.
      Item() : m_object(NULL) {}

      /// @brief Creates a new Item by copying the reference for the given
      ///        PyObject and incrementing the reference count for it.
      /// @param item The PyObject to put into the Item.
      /// @param stealReference Whether to "steal" the PyObject reference
      ///                       or increment the reference count for it.
      Item(PyObject* item, bool stealReference = false) : m_object(item) {
        if (!stealReference) { Py_XINCREF(item); }
      }

      Item(Item& item) : m_object(item.object()) {
        Py_XINCREF(m_object);
      }

      Item(const Item& item) : m_object(item.object()) {
        Py_XINCREF(m_object);
      }

      Item(Item&& moveItem) : m_object(moveItem.object()) {
        moveItem.m_object = NULL;
      }

      ~Item() {
        Py_CLEAR(m_object);
      }

      PyObject* object() const { return m_object; }

      void add_reference() { Py_XINCREF(m_object); }

      void remove_reference() { Py_XDECREF(m_object); }

      bool exists() const      { return m_object != NULL; }
      bool nonexistent() const { return m_object == NULL; }
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

      bool is_instance_of(PyObject* classObject) const {
        return PyObject_IsInstance(m_object, classObject);
      }

      bool is_subclass(PyObject* classObject) const {
        return PyObject_IsSubclass(m_object, classObject);
      }

      Item get_attr(std::string name) const {
        PyObject* attr = PyObject_GetAttrString(m_object, name.c_str());

        Item result(attr);

        // Decrement reference to not double-reference since Item() makes a
        // new reference.
        result.remove_reference();

        return result;
      }

      const char* to_string() const {
        if (m_object == NULL) { return nullptr; }

        // These create new references.
        PyObject* obString = PyObject_Str(m_object);
        PyObject* obUTF = PyUnicode_AsEncodedString(obString, "UTF-8", "~E~");

        const char* result = PyBytes_AsString(obUTF);

        // Remove reference to both itermediate PyObjects since we don't need
        // them anymore.
        Py_CLEAR(obString);
        Py_CLEAR(obUTF);

        return result;
      }

      bool to_bool() const {
        if (m_object == NULL) { return false; }

        return this->is_truthy();
      }

      int to_int() const { return PyLong_AsLong(m_object); }
      long to_long() const { return PyLong_AsLong(m_object); }

      long long to_longlong() const { return PyLong_AsLongLong(m_object); }

      unsigned long long to_ulonglong() const {
        return PyLong_AsUnsignedLongLong(m_object);
      }

      double to_double() const { return PyFloat_AsDouble(m_object); }

      friend std::ostream& operator<<(std::ostream& stream, const Item& item) {
        stream << item.to_string();
        return stream;
      }

      Boolean as_boolean();
      Float as_float();
      Int as_int();
      String as_string();

      void operator=(const Item& item) {
        this->remove_reference();

        m_object = item.m_object;

        this->add_reference();
      }

      void operator=(Item&& moveItem) {
        this->remove_reference();

        m_object = moveItem.m_object;

        moveItem.__remove_object__();
      }

      void operator()() {
        PyObject_CallNoArgs(m_object);
      }
  };
}
#endif