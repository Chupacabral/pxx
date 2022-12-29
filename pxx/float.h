#ifndef PXX_FLOAT_H
#define PXX_FLOAT_H

#include <Python.h>
#include <string>
#include <iostream>
#include "item.h"

namespace pxx {
  class Float : public Item {
    public:
      Float() { m_object = NULL; }

      Float(double n) { m_object = PyFloat_FromDouble(n); }

      Float(const char* n, int base = 10) {
        m_object = PyLong_FromString(n, NULL, base);
      }

      Float(std::string n) {
        PyObject* fmt = PyUnicode_FromString(n.c_str());

        m_object = PyFloat_FromString(fmt);

        Py_CLEAR(fmt);
      }

      Float(PyObject* object) {
        // Try to convert to double.
        double value = PyFloat_AsDouble(object);

        // New reference for new PyObject created here.
        m_object = PyFloat_FromDouble(value);
      }

      Float(Item item) {
        // TODO
      }

      Float operator -() const {
        return Float(-this->to_double());
      }

      Float operator +() const {
        long long value = this->to_double();

        if (value < 0) { value = -value; }

        return Float(value);
      }

      friend std::ostream& operator<<(std::ostream& stream, const Float& item) {
        stream << item.to_double();
        return stream;
      }
  };
}

pxx::Float operator "" _py(long double integer) {
  return pxx::Float(integer);
}

#endif