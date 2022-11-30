#ifndef PXX_LIST_H
#define PXX_LIST_H

#include <Python.h>
#include <map>
#include <string>
#include "item.h"
#include "to_pyobject.h"

// NOTE: Type "long long" used instead of "size_t" for accessing array because
//       size_t is unsigned, but ability to access array using negative
//       indices to signify access from end of array is wanted, so a signed
//       type is needed. (intmax_t from cstdint could be better, but that would
//       require importing another header).

namespace pxx {
  class List : public Item {
    public:
      // TODO: Modify List constructors to only allow definitions that allow
      //       for guaranteed correct reference count management.


      List() {
        m_object = PyList_New(0);
      }

      List(List& list) {
        m_object = list.to_object();
        this->add_reference();
      }

      List(List&& moveList) {
        m_object = moveList.to_object();
        moveList.clearObject();
      }

      List(PyObject* object) {
        // TODO: Create cases for Dicts, Tuples, etc. to make lists from them.
        if (PyList_Check(object)) {
          m_object = object;
          this->add_reference();
        }
        // If not 
        else {
          m_object = PyList_New(1);
          PyList_SetItem(m_object, 0, object);
        }
      }

      /// @brief Create an Item for a Python List object from a vector of
      ///        PyObjects.
      /// @param list The vector of PyObjects to create a Python list from.
      List(std::vector<PyObject*> list) {
        m_object = PyList_New(list.size());

        int index = 0;
        for (auto item = list.begin(); item < list.end(); item++, index++) {
          PyList_SetItem(m_object, index, *item);
          Py_XINCREF(*item);
        }
      }

      /// @brief Create an Item for a Python List object from a vector of
      ///        Items.
      /// @param list The vector of Items to create a Python list from.
      List(std::vector<Item> list) {
        m_object = PyList_New(list.size());

        int index = 0;
        for (auto item = list.begin(); item < list.end(); item++, index++) {
          PyList_SetItem(m_object, index, item->to_object());
          item->add_reference();
        }
      }

      /// @brief Create an Item for a Python List object from an initializer
      ///        list of PyObjects.
      /// @param list The list of PyObjects to create a Python list from.
      List(std::initializer_list<PyObject*> list) {
        m_object = PyList_New(list.size());

        int index = 0;
        for (auto item = list.begin(); item < list.end(); item++, index++) {
          PyList_SetItem(m_object, index, *item);
          Py_XINCREF(item);
        }
      }

      /// @brief Create an Item for a Python List object from an initializer
      ///        list of Items.
      /// @param list The list of Items to create a Python list from.
      List(std::initializer_list<Item> list) {
        m_object = PyList_New(list.size());

        int index = 0;
        for (auto item = list.begin(); item < list.end(); item++, index++) {
          PyList_SetItem(m_object, index, item->to_object());
          Py_XINCREF(item);
        }
      }

      // If it's not a vector of PyObjects or Items, then it shouldn't be
      // something that needs to track a PyObject reference to as it will be
      // creating new PyObjects.
      /* */
      /// @brief Create an Item for a Python List object from a vector of
      ///        some generic type.
      /// @tparam T The type that will be converted to Python objects for
      ///           the list (should be a type that can be made into a
      ///           Python object).
      /// @param list The vector of values to use for the list.
      template <typename T>
      List(std::vector<T> list) {
        m_object = to_pyobject(list);
      }

      // If it's not a list of PyObjects or Items, then it shouldn't be
      // something that needs to track a PyObject reference to as it will be
      // creating new PyObjects.
      /* */
      /// @brief Create an Item for a Python List object from an initializer
      ///        list of some generic type.
      /// @tparam T The type that will be converted to Python objects for
      ///           the list (should be a type that can be made into a
      ///           Python object).
      /// @param list The list of values to use for the list.
      template <typename T>
      List(std::initializer_list<T> list) {
        m_object = to_pyobject(list);
      }

      size_t size() const { return PyList_Size(m_object); }

      // NOTE: PyList_Append increments the refcount of the item itself.
      /* */
      template <typename T>
      bool append(T item) {

        // 0 means successful insertion.
        return PyList_Append(m_object, to_pyobject(item)) == 0;
      }

      // NOTE: PyList_Insert increments the refcount of the item itself.
      /* */
      template <typename T>
      bool insert(long long index, T item) {
        // Allow for negative indices by converting to positive.
        if (index < 0) { index = this->size() + index; }

        // -1 means exception for insertion.
        return PyList_Insert(m_object, index, to_pyobject(item)) != -1;
      }


      Item get(long long index) const {
        // Allow for negative indices by converting to positive.
        if (index < 0) { index = this->size() + index; }

        // PyList_GetItem returns a borrowed reference, but Item() will
        // create a strong reference for it automatically.
        return Item(PyList_GetItem(m_object, index));
      }

      Item operator[](long long index) const {
        return get(index);
      }

      Item first() const {
        // get() creates new reference.
        return this->get(0);
      }

      List rest() const {
        // slice() creates new reference.
        return this->slice(1, this->size());
      }

      Item last() const {
        // slice() creates new reference.
        return this->get(this->size() - 1);
      }

      template <typename T>
      bool set(long long index, T item) {
        // Allow for negative indices by converting to positive.
        if (index < 0) { index = this->size() + index; }

        // Copy reference for item to be removed so we can deref after.
        PyObject* temp = PyList_GetItem(index);

        // -1 means out of bounds exception.
        bool result = PyList_SetItem(m_object, index, to_pyobject(item)) != -1

        // Deref removed item.
        Py_XDECREF(temp);

        return result;
      }

      // Analogous to list[low:high]
      // Note that slicing is [low, high) access.
      List slice(long long low, long long high) const {
        // Guard clause; short-circuit return empty list.
        if (this->size() == 0) { return List(); }

        // Allow for negative indices by converting to positive.
        if (low < 0) { low = this->size() - low; }
        if (high < 0) { high = this->size() - high; }

        // PyList_GetSlice returns new reference.
        PyObject* newList = PyList_GetSlice(m_object, low, high);

        // List() adds a reference, so we need to remove it so we don't
        // double-reference item.
        auto result = List(newList);
        result.remove_reference();

        return result;
      }

      // TODO: Add duplicate to Item class; creates copy of object in a manner
      //       that updates all references as if an entirely new item was made.
      List duplicate() const {
        // OLD CODE
        // PyObject* newList = PyList_New(PyList_Size(m_object));

        // // Create a whole new Python list; increment item counts since it's
        // // a new list referencing them too.
        // for (int x = 0; x < PyList_Size(m_object); x++) {
        //   PyObject* currentItem = PyList_GetItem(m_object, x);

        //   PyList_SetItem(newList, x, currentItem);
        //   Py_XINCREF(currentItem);
        // }

        // // List() adds reference; remove added reference since we're just
        // // shoving the new list into the List wrapper.
        // List result(newList);
        // result.remove_reference();

        // This should work and make new references; test before deleting
        // old code.
        return this->slice(0, this->size());
      }

      bool sort() {
        // -1 means sort failure.
        return PyList_Sort(m_object) != -1;
      }

      List sorted() const {
        List newList = this->duplicate();
        newList.sort();

        return newList;
      }

      bool reverse() {
        // -1 means reversal failure.
        return PyList_Reverse(m_object) != -1;
      }

      List reversed() const {
        List newList = this->duplicate();
        newList.reverse();

        return newList;
      }

      // TODO: PyList_AsTuple
  };
}

#endif