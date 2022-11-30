#ifndef PXX_SET_H
#define PXX_SET_H

#include <Python.h>
#include "item.h"
#include "to_pyobject.h"

namespace pxx {
  class Set : public Item {
    public:
      Set() {
        this->object = PySet_New(NULL);
      }

      template <typename T>
      Set(std::vector<T> list) {
        PyObject* items = to_pyobject(list);

        this->object = PySet_New(items);
      }

      template <typename T>
      Set(std::initializer_list<T> list) {
        PyObject* items = to_pyobject(list);

        this->object = PySet_New(items);
      }

      size_t size() {
        return PySet_Size(this->object);
      }

      template <typename T>
      bool contains(T key) const {
        PyObject* keyObject = to_pyobject(key);

        // 1 is if key is found, 0 if not found, and -1 if error.
        return PySet_Contains(this->object, keyObject) == 1;
      }

      template <typename T>
      bool add(T key) {
        PyObject* keyObject = to_pyobject(key);

        // 0 on success.
        return PySet_Add(this->object, keyObject) == 0;
      }

      template <typename T>
      bool discard(T key) {
        PyObject* keyObject = to_pyobject(key);

        // 1 is if key is found, 0 if not found, and -1 if error.
        return PySet_Discard(this->object, keyObject) == 1;
      }

      template <typename T>
      Item pop() {
        return Item(PySet_Pop(this->object));
      }

      bool clear() {
        // TODO: Test what PySet_Clear returns on success and check that.
        return PySet_Clear(this->object);
      }
  };

  class FrozenSet : public Item {
    public:
      FrozenSet() {
        this->object = PyFrozenSet_New(NULL);
      }

      template <typename T>
      FrozenSet(std::vector<T> list) {
        PyObject* items = to_pyobject(list);

        this->object = PyFrozenSet_New(items);
      }

      template <typename T>
      FrozenSet(std::initializer_list<T> list) {
        PyObject* items = to_pyobject(list);

        this->object = PyFrozenSet_New(items);
      }

      size_t size() {
        return PySet_Size(this->object);
      }

      template <typename T>
      bool contains(T key) const {
        PyObject* keyObject = to_pyobject(key);

        // 1 is if key is found, 0 if not found, and -1 if error.
        return PySet_Contains(this->object, keyObject) == 1;
      }

      template <typename T>
      bool add(T key) {
        PyObject* keyObject = to_pyobject(key);

        // 0 on success.
        return PySet_Add(this->object, keyObject) == 0;
      }
  };
}

#endif