![](https://img.shields.io/badge/version-0.4-blue?style=for-the-badge&logo=C%2B%2B)

**NOTE:** <br>
**This library is not complete; it's usable, but every function mentioned**
**may not be implemented yet.**

# PXX

PXX is a header-only library designed to make using the Python C Api with C++
more convenient and more in-line with the language and its features.

## Features

PXX aims to reduce common boilerplate required for basic tasks when using
the Python C API.
&nbsp;
PXX utilizes C++ features to provide a more abstract and useful API,
using things such as function overloading, custom literals, and object
orientation to make code more flexible and covenient.

- You can use equivalent C++ values when creating a Python object to avoid
  a lot of boilerplate. For example, creating a list of ints with the
  base Python C API:
  ```c++
  PyObject* list = PyList_New(3);
  PyList_SetItem(list, 0, PyLong_FromLong(1));
  PyList_SetItem(list, 1, PyLong_FromLong(2));
  PyList_SetItem(list, 2, PyLong_FromLong(3));
  ```
  The same task with PXX:
  ```c++
  pxx::List list({ 1, 2, 3 });
  ```
- PXX supports custom literals for pxx objects like numbers or strings.
  Instead of:
  ```c++
  pxx::String("Hello, World!");
  ```
  if it's more convenient, you can also do:
  ```c++
  "Hello, World!"_py;
  ```

- When you start the interpreter by initializing the `pxx::Interpreter` object,
  you can give it Python files/modules to load on startup and it will keep
  track of modules you load with PXX and allow for highly abstracted code like:
  ```cpp
  int main() {
    pxx::Interpreter python({ .import = "python_file" });
    pxx::Function do_stuff("do_stuff");

    if (do_stuff.exists()) {
      pxx::String result = do_stuff().as_string();
      std::cout << result << std::endl;
    }
    
    return 0;
  }
  ```

  ```cpp
  int main() {
    Py_Initialize();
    PyObject* pypath = PySys_GetObject("path");
    PyList_Append(pypath, PyUnicode_FromString("."));

    PyObject* python_file = PyImport_ImportModule("python_file");

    if (do_stuff != NULL) {
      PyObject* do_stuff = PyObject_GetAttrString(python_file, "do_stuff");

      if (do_stuff != NULL) {
        PyObject* result = PyObject_CallNoArgs(do_stuff);
        const char* result_text = PyUnicode_AsUTF8(result);

        std::cout << result_text << std::endl;
      }
    }

    return 0;
  }
  ```

## Installation

As PXX is simply a bunch of header files, you can simply download this
repository as a zip file and put the `pxx` folder wherever you need it.

You could include whatever header(s) you want from the library, but the main
files for use are:

- `all.h`
  - This just simply loads everything in PXX.

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
If you were to put the `pxx` folder in the same folder as your main C++ file,
you would use it like:
```cpp
#include "pxx/all.h"
#include <iostream>

int main() {
  pxx::Interpreter python({ .import="code" });

  // If module or function not found, will just return pxx::None but not
  // crash. Python will still make errors in the background, though.
  auto say_greater = python["code"].get_attr("say_greater");

  // You can make pxx objects from C++ literal values by putting _py at the
  // end of numbers and strings.
  auto result = say_greater({ 3, 2 });

  // PXX classes automatically convert to their C++ equivalent values for
  // streams, so you don't need to do "result.to_string()" here.
  std::cout << "Returned value is: " << result << std::endl;

  // Can just print whatever errors happened, like if something was not found
  // somewhere in the process.
  pxx::print_errors();

  return 0;
}
```

Assuming everything is set up correctly, you should see the output:
```
x is greater
Returned value is: 1
```

<br><br>

PXX doesn't cover every possible thing in the Python C API, but it covers
all the main features (and adds extra) enough that if you just want to use Python with C++
with:
- Less boilerplate
- More C++ and less C
- "Automagic" conversion built in for you
  
Then it should be a good option.