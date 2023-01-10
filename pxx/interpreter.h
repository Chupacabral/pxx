#ifndef PXX_RUNTIME_H
#define PXX_RUNTIME_H

#include <Python.h>
#include <string>
#include <vector>
#include <map>
#include <initializer_list>

namespace pxx { class Interpreter; }

namespace pxx {
  class Item;
}

namespace pxx {
  namespace __internal__ {
    Interpreter* interpreter;
    size_t interpreterRefs = 0;
  }

  class Interpreter {
    protected:
      ::std::map<::std::string, PyObject*> m_loadedModules;

      void __set_global_interpreter() {
        pxx::__internal__::interpreter = this;
      }

      void __increment_global_interpreter() {
        pxx::__internal__::interpreterRefs++;
      }

      void __decremenet_global_interpreter() {
        pxx::__internal__::interpreterRefs--;
      }

    public:
      struct InterpreterParameters {
        int initsigs = 1;
        ::std::string import = "";
        ::std::initializer_list<::std::string> imports = {};
        ::std::string pypath = "";
      };

      /// @brief Creates an Interpreter object and initializes the Python
      ///        interpreter, turning it on.
      /// @param initsigs Value that sets how the interpreter is initialized.
      ///                 If 1, initializes normally; if 0, it skips
      ///                 initialization registration of signal handlers,
      ///                 which might be useful when Python is embedded.
      Interpreter(int initsigs = 1) {
        Py_InitializeEx(initsigs);

        __set_global_interpreter();
      }

      Interpreter(InterpreterParameters settings) {
        Py_InitializeEx(settings.initsigs);
        add_to_pypath();

        if (!settings.pypath.empty()) {
          add_to_pypath(settings.pypath);
        }

        if (!settings.import.empty()) {
          PyObject* mod = PyImport_ImportModule(settings.import.c_str());
          m_loadedModules.insert({{ settings.import, mod }});
        }

        if (settings.imports.size() > 0) {
          for (auto import : settings.imports) {
            PyObject* mod = PyImport_ImportModule(import.c_str());
            m_loadedModules.insert({{ import, mod }});
          }
        }

        __set_global_interpreter();
      }

      PyObject* get_module(::std::string name) {
        return m_loadedModules.count(name) > 0
          ? m_loadedModules[name]
          : PyImport_ImportModule(name.c_str());
      }

      PyObject* operator [](::std::string name) {
        return this->get_module(name);
      }

      /// @brief Destructs the Interpreter object, finalizing the interpreter
      ///        if still on.
      ~Interpreter() {
        if (is_initialized()) { Py_FinalizeEx(); }
      }

      /// @brief Whether the interpreter is initialized and on.
      /// @return Whether the interpreter is initialized and on.
      bool is_initialized() {
        return Py_IsInitialized();
      }

      /// @brief Whether the interpreter is finalized and off.
      /// @return Whether the interpreter is finalized and off.
      bool is_finalized() {
        return !is_initialized();
      }

      /// @brief Initializes the Python interpreter, turning it on.
      /// @param initsigs Value that sets how the interpreter is initialized.
      ///                 If 1, initializes normally; if 0, it skips
      ///                 initialization registration of signal handlers,
      ///                 which might be useful when Python is embedded.
      /// @return Whether the interpreter is initialized or not.
      bool initialize(int initsigs = 1) {
        Py_InitializeEx(initsigs);

        return is_initialized();
      }

      /// @brief Finalizes the Python interpreter, turning it off.
      /// @return Whether the interpreter is finalized or not.
      bool finalize() {
        Py_FinalizeEx();

        return is_finalized();
      }

      /// @brief Adds a given path to the pypath. Will add the current
      ///        directory by default.
      /// @param path The directory path to add the the pypath.
      /// @return Whether the path was added successfully.
      bool add_to_pypath(::std::string path = ".") {
        // Borrowed reference, do not need to deref.
        PyObject* pypath = PySys_GetObject("path");
        // New reference to new object.
        PyObject* pathString = PyUnicode_FromString(path.c_str());

        // Adds reference to pathString on append.
        bool successful = PyList_Append(pypath, pathString) == 0;

        // Remove base reference to pathString now that we're done using it.
        Py_XDECREF(pathString);

        return successful;
      }
      
      ::std::map<::std::string, PyObject*> loaded_modules() {
        return m_loadedModules;
      }
  };

  Item get_module(std::string name);
  Item get_attr(std::string module, std::initializer_list<std::string> names);
}

#endif