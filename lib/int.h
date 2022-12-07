#ifndef PXX_INT_H
#define PXX_INT_H

#include <Python.h>
#include <map>
#include <string>
#include "item.h"
#include "to_pyobject.h"

namespace pxx {
  class Int : public Item {
    public:
      Int() {
        m_object = NULL;
      }

      Int(long long n) {
        m_object = PyLong_FromLongLong(n);
      }

      Int(unsigned long long n) {
        m_object = PyLong_FromUnsignedLongLong(n);
      }

      Int(double n) {
        m_object = PyLong_FromDouble(n);
      }

      Int(const char* n, int base = 10) {
        m_object = PyLong_FromString(n, NULL, base);
      }


      Int(std::string n, int base = 10) {
        m_object = PyLong_FromString(n.c_str(), NULL, base);
      }

      template <typename T>
      Int(T n) {
        m_object = PyLong_FromLongLong(n);
      }

      /// @brief Overloaded << operator for output streams. A pxx Float will
      ///        convert to a double when inserting into stream, which will
      ///        allow it to work with IO manipulators and formatters better.
      /// @param stream The output stream to print to.
      /// @param item The pxx Float to output.
      /// @return The stream with the float printed onto it.
      friend std::ostream& operator<<(std::ostream& stream, const Float& item) {
        stream << item.to_longlong();
        return stream;
      }

      // TODO: PyLong_GetInfo after defining NamedTuple (or whatever structseq
      //       is).
  };
}

#endif