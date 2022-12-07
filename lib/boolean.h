#ifndef PXX_BOOL_H
#define PXX_BOOL_H

#include <Python.h>
#include "item.h"

namespace pxx {
  class Boolean : public Item {
    public:
      Boolean() {
        m_object = PyBool_FromLong(false);
      }

      Boolean(bool value) {
        m_object = PyBool_FromLong(value);
      }

      template <typename T>
      Boolean (T value) {
        m_object = PyBool_FromLong(value);
      }

      /// @brief Overloaded << operator for output streams. A pxx Float will
      ///        convert to a double when inserting into stream, which will
      ///        allow it to work with IO manipulators and formatters better.
      /// @param stream The output stream to print to.
      /// @param item The pxx Float to output.
      /// @return The stream with the float printed onto it.
      friend std::ostream& operator<<(std::ostream& stream, const Boolean& item) {
        stream << item.to_bool();
        return stream;
      }
  };
}

#endif