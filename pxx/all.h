#ifndef PXX_ALL_H
#define PXX_ALL_H

// #include "to_pyobject.h"
#include "interpreter.h"
#include "item.h"
#include "int.h"
#include "float.h"
#include "boolean.h"
#include "string.h"

#include "types.h"

pxx::Item pxx::get_module(std::string name) {
  PyObject* module = pxx::__internal__::interpreter != nullptr
    ? pxx::__internal__::interpreter->get_module(name)
    : NULL;
  
  return pxx::Item(module);
}

pxx::Item pxx::get_attr(std::string name, std::initializer_list<std::string> names) {
  pxx::Item baseModule = pxx::get_module(name);
  pxx::Item attr;

  if (baseModule.exists()) {
    attr = baseModule;

    for (auto name : names) {
      attr = attr.get_attr(name);

      if (attr.nonexistent()) {
        break;
      }
    }
  }

  return attr;
}

pxx::Item operator "" _pyfn(const char* text) {
  pxx::Item result;

  if (pxx::__internal__::interpreter != nullptr) {
    for (auto module : pxx::__internal__::interpreter->loaded_modules()) {
      std::string name = module.first;
      pxx::Item mod = module.second;

      pxx::Item foundFunction = mod.get_attr(text);

      if (foundFunction.exists()) {
        result = foundFunction;
        break;
      }
    }
  }

  return result;
}

#endif