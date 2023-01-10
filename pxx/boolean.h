#ifndef PXX_BOOLEAN_H
#define PXX_BOOLEAN_H

#include <Python.h>
#include <iostream>
#include "item.h"

namespace pxx {
  class Boolean : public Item {
    public:
      Boolean() { m_object = NULL; }

      Boolean(bool value) {
        m_object = PyBool_FromLong(value);
      }

      Boolean(Item item) {
        m_object = PyBool_FromLong(item.to_bool());
      }

      Boolean(const Boolean& item) {
        m_object = item.m_object;

        this->add_reference();
      }

      Boolean(Boolean& moveItem) {
        m_object = moveItem.m_object;
        moveItem.__remove_object__();
      }

      friend std::ostream& operator<<(std::ostream& stream, const Boolean& item) {
        stream
          << (item.to_bool() ? "true" : "false");
        
        return stream;
      }
  };
}

pxx::Boolean pxx::Item::as_boolean() {
  return Boolean(*this);
}

// Macros to mimic custom literals that other PXX value types have,
// as C++ does not have boolean custom literals.
#define true_py pxx::Boolean(true)
#define false_py pxx::Boolean(false)

#endif