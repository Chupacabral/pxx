#ifndef PXX_STRING_H
#define PXX_STRING_H

#include "item.h"
#include <string>
#include <iostream>

namespace pxx {
  class String : public Item {
    public:
      String() {
        m_object = NULL;
      }

      String(const char character) {
        m_object = PyUnicode_FromString(std::string(1, character).c_str());
      }

      String(std::string text) {
        m_object = PyUnicode_FromString(text.c_str());
      }

      String(Item item) {
        m_object = PyObject_Str(item.object());
      }

      String(String& item) {
        m_object = item.m_object;
        this->add_reference();
      }

      String(String&& moveItem) {
        m_object = moveItem.m_object;
        moveItem.__remove_object__();
      }

      void operator=(const String& item) {
        this->remove_reference();

        m_object = item.object();

        this->add_reference();
      }

      void operator=(String&& moveItem) {
        this->remove_reference();

        m_object = moveItem.object();

        moveItem.__remove_object__();
      }

      const char* to_string() const {
        if (m_object == NULL) { return nullptr; }

        return PyUnicode_AsUTF8(m_object);
      }

      friend std::ostream& operator<<(std::ostream& stream, const String& item) {
        stream << item.to_string();
        return stream;
      }
  };
}

pxx::String operator "" _py(const char* text) {
  return pxx::String(text);
}

pxx::String operator "" _py(const char character) {
  return pxx::String(character);
}

pxx::String pxx::Item::as_string() {
  return pxx::String(*this);
}

#endif