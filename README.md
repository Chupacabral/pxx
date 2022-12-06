![](https://img.shields.io/badge/version-0.4-blue?style=for-the-badge&logo=C%2B%2B)

**NOTE:** <br>
**This library is not complete; it's usable, but every function mentioned**
**may not be implemented yet.**

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
    - Need to make a Python value of type `X`? You can use the class `pxx::X`.
      - You can use equivalent C++ values when creating a Python object to
        avoid a lot of boilerplate. For example, creating a list of ints with
        the Python C API:
        ```c++
        PyObject* list = PyList_New(3);
        PyList_SetItem(list, 0, PyLong_FromLong(1));
        PyList_SetItem(list, 1, PyLong_FromLong(2));
        PyList_SetItem(list, 2, PyLong_FromLong(3));
        ```
        The same with PXX:
        ```c++
        pxx::List list({ 1, 2, 3 });
        ```
    - Need to get the C++ value of type `Y` from a `pxx::X` object? The method
      is called `X.to_y`.
    - Need to get an attribute? Instead of `PyObject_GetAttrString`, it's just
      `X.get_attr`
      - Instead of a bunch of boilerplate just to grab a function, you can do
        it in a single line like:
        ```c++
        auto fn = pxx::Module("stuff").get_attr("cool function");
        ```
        If something is not found, `get_attr` will just return a null object
        instead of throwing an error, allowing a sort of safe navigation when
        trying to access members

- Allows Using Common C++ Types
  - As the Python API is in C, it does not use common container types like
    vectors, strings, or maps. PXX allows for use of these types with the API:
    - Functions that take or return strings can use `std::string` instead of
      `const char*`; if you want C-strings, the relevant functions will have
      a version with `cstring` in the name.
    - Functions involving lists will allow for use of `std::vector` (or
      `std::initializer_list` if you want to give an inline list literal to
      a function).
      - The `pxx::List::to_vector` function is perfect for converting a Python list
        into a C++ value.
    - Functions involving dictionaries will allow the use of `std::map`
      (map will be the default, as it keeps track of order like Python's
       dictionary does).
      - `to_map` is the dictionary version of `to_vector`
    - If you *really* need to use another container type, you can use variants
      of the aforemention methods that takes a lambda to convert the list/dict
      to the container type
      - `pxx::List::convert` or `pxx::Dict::convert`

- Actual Object Orientation
  - As C++ allows for actual objects, so does PXX. For all the main types of
    objects in Python, there is a class version of it.
    
    The possible object types in PXX are:

    - **`Item`**
      - Much like how all Python C API objects use `PyObject`, this is the base
        class that all PXX objects are based on. <br>
        Has all the base methods for checking what kind of Python object
        the value is, converting it to a C++ value, etc.

        All methods that return a PXX object will return an `Item`, so
        if you want a specific type of object you should either use that
        specific type or convert it like:
        ```c++
        pxx::Module shopping("shopping");
        pxx::List shopping_list(shopping.get_attr("shopping_list"));
        ```
    
    - Containers: **`List`**, **`Dict`**, **`Tuple`**, **`Set`**
    - Values: **`Int`**, **`Float`**, **`Boolean`**, **`String`**, **`None`**
    - Other: **`Module`**, **`Class`**, **`Object`**, **`Function`**,
             **`Error`**
    - **`Interpreter`**
      - This class is for, well, the Python interpreter. <br>
        Creation and deletion will run and close the interpreter automatically
        for you, but you can also manually do either whenever you want without
        a problem.

- Better Integration with C++ Code
  - Not only do PXX objects use the aforementioned object orientation and
    common C++ standard library types, but they are made to be used in C++
    code themselves with a few key features:
    - **Operator Overloading**
      - You can compare the Python values within PXX objects by just using the
        relevant operator:
        ```cpp
        pxx::Int x(1), y(3);

        if (x < y) { /* CODE */ }  // Compares like 1 < 3
        ```
      - PXX objects have a `to_string` method, but if you need it to print
        some output, you can just give the object to the output stream:
        ```cpp
        pxx::Int coolNumber(23);

        std::cout << coolNumber;  // prints "23"
        ```
        Note that types like `Float` will actually convert to the related
        C++ value (in that case, `double`), which will allow them to work
        with IO manipulators and formatters better.
    - **Iterators**
      - Container types implement iterators, meaning you can do a range-based
        for-loop on the PXX object itself, with no conversion or manual
        reading required:
        ```cpp
        pxx::List numbers({ 1, 2, 3, 4, 5 })

        for (auto number : numbers) {
          std::cout << number << " ";
        }
        // Prints "1 2 3 4 5 "
        ```
    - **Copy and Move Semantics**
      - PXX objects automatically track their references to the underlying
        Python objects, and implement the relevant copy and move methods
        to ensure they work with C++ to track the lifetime of these references
        the best they can.

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
  auto python = pxx::Interpreter();
  python.add_dir_to_path(); // This ensures your .py file is loaded.

  // If module or function not found, will just return pxx::None but not
  // crash. Python will still make errors in the background, though.
  auto say_greater = pxx::Module("code").get_attr("say_greater");

  // This will also return pxx::None if say_greater was not found.
  auto result = say_greater({ pxx::Int(3), pxx::Int(2) });

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