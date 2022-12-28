#ifndef PXX_TYPES_H
#define PXX_TYPES_H

/*
Allows for convenience of compressing all PXX type names used in code by
removing the pxx:: prefix, but without shoving the entire pxx namespace
into the global namespace.
*/

namespace pxx {
  class Interpreter;
  class Item;
  class Int;
  class Float;
  class Boolean;
  class String;
  class List;
  class Dict;
}

namespace pxx {
  namespace types {
    typedef pxx::Interpreter Interpreter;
    typedef pxx::Item Item;
    typedef pxx::Int Int;
    typedef pxx::Float Float;
    typedef pxx::Boolean Boolean;
    typedef pxx::String String;
    typedef pxx::List List;
    typedef pxx::Dict Dict;
  }
}

#endif