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

      Float(Item item) {
        m_object = PyFloat_FromDouble(item.to_double());
      }

      Float(const Float& item) {
        m_object = item.m_object;
        this->add_reference();
      }

      Float(Float&& moveItem) {
        m_object = moveItem.m_object;
        moveItem.__remove_object__();
      }

      Float operator -() const {
        return Float(-this->to_double());
      }

      Float operator +() const {
        long long value = this->to_double();

        if (value < 0) { value = -value; }

        return Float(value);
      }

      void operator=(const Float& item) {
        this->remove_reference();

        m_object = item.object();

        this->add_reference();
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

pxx::Float pxx::Item::as_float() {
  return pxx::Float(*this);
}

#endif