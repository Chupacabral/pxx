#ifndef PXX_PY_TYPES_H
#define PXX_PY_TYPES_H

#include <Python.h>

namespace py {
  typedef PyObject* Function;
  typedef PyObject* Module;
  typedef PyObject* Int;
  typedef PyObject* Float;
  typedef PyObject* String;
  typedef PyObject* Boolean;
  typedef PyObject* List;
  typedef PyObject* Tuple;
  typedef PyObject* Set;
  typedef PyObject* Dict;
  typedef PyObject* Object;
  typedef PyObject* Type;
  typedef PyObject* Error;
  typedef PyObject* Unknown;
  typedef PyObject* Any;
}

#endif