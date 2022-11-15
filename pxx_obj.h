#ifndef PXX_OBJ_H
#define PXX_OBJ_H

// This file is for the wrapper object part of the PXX library.
// Parts of the Object class were inspired or used from
// https://github.com/nucleic/cppy/blob/main/cppy/include/cppy/ptr.h
// from the "cppy" C++ Python object wrapper API.
#include "pxx_py.h"
#include "pxx_lib.h"

namespace pxx {
  template <typename T>
  T* Deref(T* ob) {
    Py_DECREF(ob);

    return ob;
  }

  class Object {
    protected:
      py::Any ob;
    
    public:
      Object(): ob(NULL) {}

      Object(py::Any object): ob(object) {}

      ~Object() {
        PyObject* temp = this->ob;
        this->ob = NULL;

        Py_XDECREF(temp);
      }

      Object& operator=(py::Any other) {
        PyObject* temp = this->ob;
        this->ob = other;

        Py_XDECREF(temp);

        return *this;
      }

      Object& operator=(Object& other) {
        PyObject* temp = this->ob;
        this->ob = other.Unwrap();

        Py_XINCREF(this->ob);
        Py_XDECREF(temp);

        return *this;
      }

      py::Any Unwrap() { return this->ob; }

      py::Any Release() {
        PyObject* temp = this->ob;
        this->ob = NULL;

        return temp;
      }

      operator void*() const {
        return static_cast<void*>(this->ob);
      }

      bool IsNone()     const { return pxx::IsNone(this->ob);     }
      bool IsTrue()     const { return pxx::IsTrue(this->ob);     }
      bool IsFalse()    const { return pxx::IsFalse(this->ob);    }
      bool IsTruthy()   const { return pxx::IsTruthy(this->ob);   }
      bool IsFalsy()    const { return pxx::IsFalsy(this->ob);    }
      bool IsBoolean()  const { return pxx::IsBoolean(this->ob);  }
      bool IsInteger()  const { return pxx::IsInteger(this->ob);  }
      bool IsFloat()    const { return pxx::IsFloat(this->ob);    }
      bool IsList()     const { return pxx::IsList(this->ob);     }
      bool IsDict()     const { return pxx::IsDict(this->ob);     }
      bool IsSet()      const { return pxx::IsSet(this->ob);      }
      bool IsBytes()    const { return pxx::IsBytes(this->ob);    }
      bool IsString()   const { return pxx::IsString(this->ob);   }
      bool IsCallable() const { return pxx::IsCallable(this->ob); }
      bool IsIterable() const { return pxx::IsIterable(this->ob); }

      bool IsType(PyTypeObject* type) const {
        return pxx::IsType(this->ob, type);
      }

      bool IsInstanceOf(py::Type cls) const {
        return pxx::IsInstanceOf(this->ob, cls);
      }

      bool IsSubclass(py::Type cls) const {
        return pxx::IsSubclass(this->ob, cls);
      }

      Object Call(py::Tuple args) {
        py::Unknown result = PyObject_CallObject(this->ob, args);

        return Object(result);
      }

      Object Call(Object args) {
        py::Unknown argObject = args.Unwrap();

        //? TODO: ERR IF NOT TUPLE OR LIST?

        return this->Call(
          args.IsList()
            ? PyList_AsTuple(argObject)
            : argObject
        );
      }

      Object operator()(py::Tuple args) {
        return this->Call(args);
      }

      Object operator()(Object args) {
        return this->Call(args);
      }
  };

  class Dict {
    private:
      py::Dictionary ob;
    public:
      Dict() { this->ob = PyDict_New(); }

      Dict Set(py::Any key, py::Any value) {
        PyDict_SetItem(this->ob, key, value);
        return *this;
      }

      Dict Set(std::string key, py::Any value) {
        PyDict_SetItemString(this->ob, key.c_str(), value);
        return *this;
      }

      py::Dictionary Object() { return this->ob; }
  };
}

#endif