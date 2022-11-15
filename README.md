![](https://img.shields.io/badge/version-0.3-blue?style=for-the-badge&logo=C%2B%2B)

# PXX

PXX is a header-only library designed to make using the Python C Api with C++
more convenient and more in line with the language.

## Features

- More convenient
  - PXX tries to reduce the boilerplate for basic tasks that would happen when
    using the Python C API while not rewriting the whole thing.
  - PXX uses C++ language features to provide a more abstract and powerful API,
    utilizing function overloading and features like initialization lists to
    make functions more flexible and convenient.
  - Straightforward API:
    - Need to make a Python value of type `X`? The function is called `Make[X]`
    - Need to get the C++ value of type `X`? The function is called `To[X]`
    - Need to get an attribute? Instead of `PyObject_GetAttrString`, it's just
      `pxx::GetAttribute`
  
- Useful Type Aliases
  - The Python C API, being made in C without the ability to make classes like
    C++ allows, puts every Python value in the generic type `PyObject`. While
    PXX does not magically fix this, it does provide useful aliases over
    `PyObject` to make the intent of your code more readable.
  - For example, if you expect a function, you can use the type `py::Function`
    instead of `PyObject*` to show you expect a Python function.

- Allows Using Common C++ Types
  - As the Python API is in C, it does not use common container types like
    vectors, strings, or maps. PXX allows for use of these types with the API:
    - Functions that take or return strings can use `std::string` instead of
      `const char*`; if you want C-strings, the relevant functions will have
      a version with `CString` in the name (like `pxx::ToCString`).
    - Functions involving lists will allow for use of `std::vector` (or
      `std::initializer_list` if you want to give an inline list literal to
      a function).
      - The `pxx::ToVector` function is perfect for converting a Python list
        into a C++ value.
    - Functions involving dictionaries will allow the use of `std::map` or
      `std::unordered_map` (map will be the default, as it keeps track of order
      like Python's dictionary does).
      - `pxx::ToMap` is the dictionary version of `pxx::ToVector`
    - If you *really* need to use another container type, you can use variants
      of the aforemention methods that takes a lambda to convert the list/dict
      to the container type
      - `pxx::ConvertList` and `pxx::ConvertDict`
## Installation

Download the library as a zip file, unzip, and get the header files from the
folder.

You can put the files wherever you want, as long as you make sure your
compiler knows where they are when you compile your code.

Putting the library with your code is the easy part; you'll need to know how
to properly include Python when compiling your C++ code.

## Using PXX

Using PXX is relatively simple, as it's just a header library.

For this example, let's assume we have a Python file `code.py` in our folder:
```py
def say_greater(x, y):
  if x == y:
    print('x equals y')
    return 0
  elif x > y:
    print('x is greater')
    return 1
  else:
    print('y is greater')
    return -1

```
If you were to put the pxx files in the same folder as your main C++ file,
you would use it like:
```cpp
#include "pxx.h"
#include <iostream>

int main() {
  pxx::InitializeInterpreter();
  pxx::LoadDirectoryInPython(); // This ensures your .py file is loaded.

  py::Function say_greater = pxx::GetAttribute("code", "say_greater");

  if (say_greater) {
    py::Int result = pxx::CallFunction(say_greater, {
      pxx::MakeInt(3), pxx::MakeInt(2)
    });

    if (result) {
      std::cout << "Returned value is: " << pxx::ToString(result) << std::endl;
    }
    else { pxx::PrintError(); }
  }
  else { pxx::PrintError(); }

  pxx::FinalizeInterpreter();

  return 0;
}
```

Assuming everything is set up correctly, you should see the output:
```
x is greater
Returned value is: 3
```