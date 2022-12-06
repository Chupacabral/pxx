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

      /// @brief Overloaded << operator for output streams. A pxx Float will
      ///        convert to a double when inserting into stream, which will
      ///        allow it to work with IO manipulators and formatters better.
      /// @param stream The output stream to print to.
      /// @param item The pxx Float to output.
      /// @return The stream with the float printed onto it.
      friend std::ostream& operator<<(std::ostream& stream, const Float& item) {
        stream << item.to_double();
        return stream;
      }

      // TODO: PyFloat_GetInfo after defining NamedTuple (or whatever structseq
      //       is).
  };
}

#endif