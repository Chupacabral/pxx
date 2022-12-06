#ifndef PXX_ITEM_H
#define PXX_ITEM_H

#include <Python.h>
#include "to_pyobject.h"
#include <vector>
#include <map>
#include <iostream>

namespace pxx {
  class Item {
    protected:
      PyObject* m_object;

      void clearObject() {
        m_object = NULL;
      }
    
    public:
      
      /// @brief Creates a new Item with NULL as the Python object inside.
      Item() : m_object(NULL) {}

      /// @brief Creates a new Item by copying the reference for the given
      ///        PyObject and incrementing the reference count for it.
      /// @param item The PyObject to put into the Item.
      /// @param stealReference Whether to "steal" the PyObject reference
      ///                       or increment the reference count for it.
      Item(PyObject* item, bool stealReference = false) : m_object(item) {
        if (!stealReference) {
          Py_XINCREF(item);
        }
      }

      /// @brief Copy constructor for an Item; copies the Python object in the
      ///        Item given into the new item and increments the reference
      ///        count for it.
      /// @param item The Item to copy from.
      Item(const Item& item) : m_object(item.to_object()) {
        // Increment reference count as we have copied reference.
        // Will cancel out with decrement when object destructed.
        Py_XINCREF(m_object);
      }

      /// @brief Move constructor for an Item; takes the Python object in the
      ///        Item given into the new Item.
      /// @param moveItem The item to move the Python object from.
      Item(Item&& moveItem) : m_object(moveItem.to_object()) {
        moveItem.clearObject();
      }

      // Generic "create PyObject*" constructor for Item.
      // Do not manipulate reference here, as it implicitly makes the reference
      // since it makes a new PyObject* for construction.
      /* */
      /// @brief Generic Item constructor that makes the underlying
      ///        Python object using the value given.
      /// @tparam T Any type that is usable to make a Python object.
      /// @param item The item to use for the new Python object.
      template <typename T>
      Item(T item) : m_object(to_pyobject(item)) {}

      // Destructor.
      ~Item() {
        // Decrement reference count for PyObject and set variable to
        // NULL.
        Py_CLEAR(m_object);
      }

      /// @brief Increments the reference count for the Python object within
      ///        the Item.
      ///        DO NOT USE THIS UNLESS YOU ARE ABSOLUTELY SURE YOU NEED TO.
      ///        PXX ITEMS AUTOMATICALLY MANAGE THEIR REFERENCE COUNTS.
      void add_reference() { Py_XINCREF(m_object); }

      /// @brief Decrements the reference count for the Python object within
      ///        the Item.
      ///        DO NOT USE THIS UNLESS YOU ARE ABSOLUTELY SURE YOU NEED TO.
      ///        PXX ITEMS AUTOMATICALLY MANAGE THEIR REFERENCE COUNTS.
      void remove_reference() { Py_XDECREF(m_object); }

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
      Item get_attr(T attrName) const {
        // Returns new reference.
        PyObject* attr = PyObject_GetAttr(m_object, to_pyobject(attrName));

        auto result = Item(attr);

        // Decrement reference to not double-reference since Item() makes a
        // new reference.
        Py_XDECREF(attr);

        return result;
      }

      std::string to_string() const {
        if (m_object == NULL) { return nullptr; }

        // These create new references.
        PyObject* obString = PyObject_Str(m_object);
        PyObject* obUTF = PyUnicode_AsEncodedString(obString, "UTF-8", "~E~");

        std::string result = PyBytes_AsString(obUTF);

        // Remove reference to both itermediate PyObjects since we don't need
        // them anymore.
        Py_CLEAR(obString);
        Py_CLEAR(obUTF);

        return result;
      }

      const char* to_cstring() const {
        if (m_object == NULL) { return NULL; }

        // These create new references.
        PyObject* obString = PyObject_Str(m_object);
        PyObject* obUTF = PyUnicode_AsEncodedString(obString, "UTF-8", "~E~");

        const char* result = PyBytes_AsString(obUTF);

        // Remove reference to both itermediate PyObjects since we don't need
        // them anymore.
        Py_CLEAR(obString);
        Py_CLEAR(obUTF);

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
        return std::vector<Item>({ this });
      }

      std::map<Item, Item> to_map() const {
        return std::map<Item, Item>({{ this, this }});
      }

      void operator=(PyObject* item) {
        m_object = item;
        Py_XINCREF(item);
      }

      void operator=(const Item& item) {
        m_object = item.to_object();

        // Increment reference count as we have copied reference.
        // Will cancel out with decrement when object destructed.
        Py_XINCREF(m_object);
      }

      void operator=(Item&& moveItem) {
        m_object = moveItem.to_object();
        moveItem.clearObject();
      }

      bool operator<(Item& item) const {
        return PyObject_RichCompareBool(m_object, item.to_object(), Py_LT) == 1;
      }

      bool operator<(PyObject* item) const {
        return PyObject_RichCompareBool(m_object, item, Py_LT) == 1;
      }

      template <typename T>
      bool operator<(T item) const {
        return PyObject_RichCompareBool(m_object, to_pyobject(item), Py_LT) == 1;
      }

      bool operator!=(Item& item) const {
        return PyObject_RichCompareBool(m_object, item.to_object(), Py_NE) == 1;
      }

      bool operator!=(PyObject* item) const {
        return PyObject_RichCompareBool(m_object, item, Py_NE) == 1;
      }

      template <typename T>
      bool operator!=(T item) const {
        return PyObject_RichCompareBool(m_object, to_pyobject(item), Py_NE) == 1;
      }

      /// @brief Overloaded << operator for output streams. For a base pxx
      ///        Item, will just use to_string().
      /// @param stream The output stream to print to.
      /// @param item The pxx Item to output.
      /// @return The stream with the item printed onto it.
      friend std::ostream& operator<<(std::ostream& stream, const Item& item) {
        stream << item.to_string();
        return stream;
      }
  };
}

#endif