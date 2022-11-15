#ifndef PXX_PY_H
#define PXX_PY_H

// This file is for definitions of all the alias types used by the PXX library
// to better describe the intentions behind all the PyObjects being passed
// around.
#include <Python.h>

namespace py {
  /** Type alias for PyObject* used to declare intent of being an integer. */
  typedef PyObject* Integer;
  /** Type alias for PyObject* used to declare intent of being a float. */
  typedef PyObject* Float;
  /** Type alias for PyObject* used to declare intent of being a string. */
  typedef PyObject* String;
  /** Type alias for PyObject* used to declare intent of being a boolean. */
  typedef PyObject* Boolean;
  /** Type alias for PyObject* used to declare intent of being a list. */
  typedef PyObject* List;
  /** Type alias for PyObject* used to declare intent of being a tuple. */
  typedef PyObject* Tuple;
  /** Type alias for PyObject* used to declare intent of being a set. */
  typedef PyObject* Set;
  /** Type alias for PyObject* used to declare intent of being a dictionary. */
  typedef PyObject* Dictionary;
  /** Type alias for PyObject* used to declare intent of being a function. */
  typedef PyObject* Function;
  /** Type alias for PyObject* used to declare intent of being a module. */
  typedef PyObject* Module;
  /** Type alias for PyObject* used to declare intent of being an object. */
  typedef PyObject* Object;
  /** Type alias for PyObject* used to declare intent of being a type. */
  typedef PyObject* Type;
  /** Type alias for PyObject* used to declare intent of being error value. */
  typedef PyObject* Error;
  /** Type alias for PyObject* used to declare no explicit knowledge of type. */
  typedef PyObject* Unknown;
  /** Type alias for PyObject* used to declare that it can be py::Any PyObject */
  typedef PyObject* Any;
}

#endif