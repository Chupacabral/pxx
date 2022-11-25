#ifndef PXX_BOOL_H
#define PXX_BOOL_H

#include "py_types.h"
#include "item.h"

namespace pxx {
  class Boolean : public Item {
    public:
      Boolean() {
        this->object = PyBool_FromLong(false);
      }

      Boolean(bool value) {
        this->object = PyBool_FromLong(value);
      }

      template <typename T>
      Boolean (T value) {
        this->object = PyBool_FromLong(value);
      }
  };
}

#endif