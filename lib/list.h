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
      List() {
        m_object = PyList_New(0);
      }

      List(PyObject* object) {
        if (PyList_Check(object)) {
          m_object = object;
        }
        else {
          m_object = PyList_New(1);
          PyList_SetItem(m_object, 0, object);
        }
      }

      template <typename T>
      List(std::vector<T> list) {
        m_object = to_pyobject(list);
      }

      template <typename T>
      List(std::initializer_list<T> list) {
        m_object = to_pyobject(list);
      }

      size_t size() const { return PyList_Size(m_object); }

      template <typename T>
      bool append(T item) {
        // 0 means successful insertion.
        return PyList_Append(m_object, to_pyobject(item)) == 0;
      }

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

        return Item(PyList_GetItem(m_object, index));
      }

      Item operator[](long long index) const {
        return get(index);
      }

      template <typename T>
      bool set(long long index, T item) {
        // Allow for negative indices by converting to positive.
        if (index < 0) { index = this->size() + index; }

        // -1 means out of bounds exception.
        return PyList_SetItem(m_object, index, to_pyobject(item)) != -1;
      }

      List slice(long long low, long long high) {
        // Guard clause; short-circuit return empty list.
        if (this->size() == 0) { return List(); }

        // Allow for negative indices by converting to positive.
        if (low < 0) { low = this->size() - low; }
        if (high < 0) { high = this->size() - high; }

        PyObject* newList = PyList_GetSlice(m_object, low, high);

        return List(newList);
      }

      bool sort() {
        // -1 means sort failure.
        return PyList_Sort(m_object) != -1;
      }

      bool reverse() {
        // -1 means reversal failure.
        return PyList_Reverse(m_object) != -1;
      }

      // TODO: PyList_AsTuple
  };
}

#endif