#ifndef PXX_TUPLE_H
#define PXX_TUPLE_H

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
  class Tuple : public Item {
    public:
      Tuple() {
        this->object = PyTuple_New(0);
      }

      Tuple(PyObject* object) {
        if (PyTuple_Check(object)) {
          this->object = object;
        }
        else {
          this->object = PyTuple_Pack(1, object);
        }
      }

      template <typename T>
      Tuple(std::vector<T> list) {
        this->object = PyTuple_New(list.size());

        for (int x = 0; x < list.size(); x++) {
          PyObject* newObject = to_pyobject(list.at(x));
          PyTuple_SetItem(this->object, x, newObject);
        }
      }

      template <typename T>
      Tuple(std::initializer_list<T> list) {
        this->object = PyTuple_New(list.size());

        for (int x = 0; x < list.size(); x++) {
          PyObject* newObject = to_pyobject(list.at(x));
          PyTuple_SetItem(this->object, x, newObject);
        }
      }

      size_t size() const { return PyTuple_Size(this->object); }

      Item get(long long index) const {
        // Allow for negative indices by converting to positive.
        if (index < 0) { index = this->size() + index; }

        return Item(PyTuple_GetItem(this->object, index));
      }

      Item operator[](long long index) const {
        return get(index);
      }

      template <typename T>
      bool set(long long index, T item) {
        // Allow for negative indices by converting to positive.
        if (index < 0) { index = this->size() + index; }

        // -1 means insertion error.
        return PyTuple_SetItem(this->object, index, to_pyobject(item)) != -1;
      }

      // TODO: to_list()
  };
}

#endif