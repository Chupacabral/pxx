#ifndef PXX_DICT_H
#define PXX_DICT_H

#include <Python.h>
#include <map>
#include <string>
#include "item.h"
#include "to_pyobject.h"

namespace pxx {
  class Dict : public Item {
    public:
      Dict() {
        this->object = PyDict_New();
      }

      template <typename K, typename V>
      Dict(std::map<K, V> dict) {
        this->object = to_pyobject(dict);
      }

      template <typename K>
      Item get(K key, bool errors = false) const {
        PyObject* keyObject = to_pyobject(key);

        return errors
          ? PyDict_GetItem(this->object, keyObject)
          : PyDict_GetItemWithError(this->object, keyObject);
      }

      template <typename K>
      Item operator[](K key) const {
        PyObject* foundItem = PyDict_GetItem(this->object, to_pyobject(key));
        return Item(foundItem);
      }

      template <typename K, typename V>
      bool set(K key, V value) {
        PyObject* keyObject = to_pyobject(key);
        PyObject* valObject = to_pyobject(value);

        return PyDict_SetItem(this->object, keyObject, valObject) == 0;
      }

      template <typename K>
      bool remove(K key) {
        return PyDict_DelItem(this->object, to_pyobject(key)) == 0;
      }

      // TODO: Make this in file after all items are defined:
      //       (This should work when ready)
      //
      // pxx::List pxx::Dict::items() {
      //   PyObject* keys = PyDict_Keys(this->object);
      //   PyObject* items = PyList_New(0);

      //   for (int x = 0; x < PyList_Size(keys); x++) {
      //     PyObject* pair = PyTuple_New(2);
      //     PyObject* key = PyList_GetItem(keys, x);
      //     PyObject* value = PyDict_GetItem(this->object, key);

      //     PyTuple_SetItem(pair, 0, key);
      //     PyTuple_SetItem(pair, 1, value);

      //     PyList_Append(items, pair);
      //   }

      //   return List(items);
      // }

      std::map<Item, Item> to_map() const {
        PyObject* keys = PyDict_Keys(this->object);
        std::map<Item, Item> result;

        for (int x = 0; x < PyList_Size(keys); x++) {
          PyObject* key = PyList_GetItem(keys, x);
          PyObject* value = PyDict_GetItem(this->object, key);

          result.insert({{ Item(key), Item(value) }});
        }

        return result;
      }
  };
}
#endif