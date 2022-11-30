#ifndef PXX_FLOAT_H
#define PXX_FLOAT_H

#include <Python.h>
#include <map>
#include <string>
#include "item.h"
#include "to_pyobject.h"

namespace pxx {
  class Float : public Item {
    public:
      double MAX = PyFloat_GetMax();
      double MIN = PyFloat_GetMin();
      
      Float() {
        m_object = NULL;
      }

      Float(double n) {
        m_object = PyFloat_FromDouble(n);
      }

      Float(const char* n) {
        m_object = PyFloat_FromString(to_pyobject(n));
      }

      Float(std::string n) {
        m_object = PyFloat_FromString(to_pyobject(n));
      }

      template <typename T>
      Float(T n) {
        m_object = PyFloat_FromDouble(n);
      }

      // TODO: PyFloat_GetInfo after defining NamedTuple (or whatever structseq
      //       is).
  };
}

#endif