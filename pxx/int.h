#ifndef PXX_INT_H
#define PXX_INT_H

#include <Python.h>
#include <string>
#include <iostream>
#include "item.h"

namespace pxx {
  class Int : public Item {
    public:
      Int() { m_object = NULL; }

      Int(long long n) { m_object = PyLong_FromLongLong(n); }

      Int(const char* n, int base = 10) {
        m_object = PyLong_FromString(n, NULL, base);
      }

      Int(std::string n, int base = 10) {
        m_object = PyLong_FromString(n.c_str(), NULL, base);
      }

      Int(PyObject* object) {
        long long intValue = PyLong_AsLongLong(object);

        // New reference for new PyObject created here.
        m_object = PyLong_FromLongLong(intValue);
      }

      Int(Item item) {
        long long intValue = item.to_longlong();

        // New reference for new PyObject created here.
        m_object = PyLong_FromLongLong(intValue);
      }

      Int operator -() const {
        return Int(-this->to_longlong());
      }

      Int operator +() const {
        long long value = this->to_longlong();

        if (value < 0) { value = -value; }

        return Int(value);
      }

      friend std::ostream& operator<<(std::ostream& stream, const Int& item) {
        stream << item.to_longlong();
        return stream;
      }
  };
}

pxx::Int operator "" _py(unsigned long long integer) {
  return pxx::Int(integer);
}

#endif