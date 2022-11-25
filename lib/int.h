#ifndef PXX_INT_H
#define PXX_INT_H

#include "py_types.h"
#include "item.h"
#include "to_pyobject.h"
#include <map>
#include <string>

namespace pxx {
  class Int : public Item {
    public:
      Int() {
        this->object = NULL;
      }

      Int(long long n) {
        this->object = PyLong_FromLongLong(n);
      }

      Int(unsigned long long n) {
        this->object = PyLong_FromUnsignedLongLong(n);
      }

      Int(double n) {
        this->object = PyLong_FromDouble(n);
      }

      Int(const char* n, int base = 10) {
        this->object = PyLong_FromString(n, NULL, base);
      }


      Int(std::string n, int base = 10) {
        this->object = PyLong_FromString(n.c_str(), NULL, base);
      }

      template <typename T>
      Int(T n) {
        this->object = PyLong_FromLongLong(n);
      }

      // TODO: PyLong_GetInfo after defining NamedTuple (or whatever structseq
      //       is).
  };
}

#endif