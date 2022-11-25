#ifndef PXX_FLOAT_H
#define PXX_FLOAT_H

#include "py_types.h"
#include "item.h"
#include "to_pyobject.h"
#include <map>
#include <string>

namespace pxx {
  class Float : public Item {
    public:
      double MAX = PyFloat_GetMax();
      double MIN = PyFloat_GetMin();
      
      Float() {
        this->object = NULL;
      }

      Float(double n) {
        this->object = PyFloat_FromDouble(n);
      }

      Float(const char* n) {
        this->object = PyFloat_FromString(to_pyobject(n));
      }

      Float(std::string n) {
        this->object = PyFloat_FromString(to_pyobject(n));
      }

      template <typename T>
      Float(T n) {
        this->object = PyFloat_FromDouble(n);
      }

      // TODO: PyFloat_GetInfo after defining NamedTuple (or whatever structseq
      //       is).
  };
}

#endif