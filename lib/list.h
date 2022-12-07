#ifndef PXX_LIST_H
#define PXX_LIST_H

#include <Python.h>
#include <map>
#include <string>
#include "item.h"
#include "to_pyobject.h"

// NOTE: Type "long long" used instead of "size_t" for accessing array because
//       size_t is unsigned, but ability to access array using negative
//       indices to signify access from end of array is wanted, so a signed
//       type is needed. (intmax_t from cstdint could be better, but that would
//       require importing another header).

namespace pxx {

  class List : public Item {
    private:
      class Iterator {
        protected:
          const List* m_list;
          Item m_currentItem;
          size_t m_index;
        
        public:
          Iterator(List* list, size_t index = 0) : m_list(list) {
            m_currentItem = index < m_list->size()
              ? m_list->get(index)
              : Item(); 
            
            m_index = index;
          }

          Iterator(const List* list, size_t index = 0) : m_list(list) {
            m_currentItem = index < m_list->size()
              ? m_list->get(index)
              : Item();
            
            m_index = index;
          }

          bool operator!=(const Iterator other) const {
            return m_currentItem.to_object() != other.m_currentItem.to_object();
          }

          Item operator*() { return m_currentItem; }
          Item operator->() { return m_currentItem; }
          const Item operator*() const { return m_currentItem; }

          void operator++() {
            m_index++;

            m_currentItem = m_index < m_list->size()
              ? m_list->get(m_index)
              : Item();
          }
      };

    public:
      /// @brief Creates a new List with an empty Python list object inside.
      List() {
        m_object = PyList_New(0);
      }

      /// @brief Copy constructor for a List; copies the Python list object
      ///        in the List into the new List and increments the reference
      ///        count for it.
      /// @param list The List object to copy from.
      List(List& list) {
        m_object = list.to_object();
        this->add_reference();
      }

      /// @brief Move constructor for a new List; takes the Python list object
      ///        in the List and puts it in the new List.
      /// @param moveList The List to move the Python list object from.
      List(List&& moveList) {
        m_object = moveList.to_object();
        moveList.clearObject();
      }

      /// @brief Creates a new List based on the given PyObject.
      ///        Iterable Python objects (List, Tuple, Dict, and subtypes)
      ///        will be used to construct the underlying Python list object.
      ///        Value-type objects will be simply be put in a new List.
      /// @param object The Python object to use when making the list.
      /// @param stealReference Whether to steal the reference for the given
      ///                       PyObject, or incremenet the reference count.
      List(PyObject* object, bool stealReference = false) {
        // TODO: Create cases for Dicts, Tuples, etc. to make lists from them.
        if (PyList_Check(object)) {
          m_object = object;

          if (!stealReference) { this->add_reference(); }
        }
        // If not, simply construct a new list with the object as a member.
        else {
          m_object = PyList_New(1);
          // Steals Item reference.
          PyList_SetItem(m_object, 0, object);

          // Add reference to counter steal, if wanted.
          if (!stealReference) { Py_XINCREF(object); }
        }
      }

      /// @brief Create an Item for a Python List object from a vector of
      ///        PyObjects.
      /// @param list The vector of PyObjects to create a Python list from.
      List(std::vector<PyObject*> list, bool stealReferences = false) {
        m_object = to_pyobject(list, stealReferences);
      }

      /// @brief Create an Item for a Python List object from a vector of
      ///        Items.
      /// @param list The vector of Items to create a Python list from.
      List(std::vector<Item> list, bool stealReferences = false) {
        m_object = to_pyobject(list, stealReferences);
      }

      /// @brief Create an Item for a Python List object from an initializer
      ///        list of PyObjects.
      /// @param list The list of PyObjects to create a Python list from.
      List(std::initializer_list<PyObject*> list, bool stealReferences = false) {
        m_object = to_pyobject(list, stealReferences);
      }

      /// @brief Create an Item for a Python List object from an initializer
      ///        list of Items.
      /// @param list The list of Items to create a Python list from.
      List(std::initializer_list<Item> list, bool stealReferences = false) {
        m_object = to_pyobject(list, stealReferences);
      }

      // If it's not a vector of PyObjects or Items, then it shouldn't be
      // something that needs to track a PyObject reference to as it will be
      // creating new PyObjects.
      /* */
      /// @brief Create an Item for a Python List object from a vector of
      ///        some generic type.
      /// @tparam <T> The type that will be converted to Python objects for
      ///             the list (should be a type that can be made into a
      ///             Python object).
      /// @param list The vector of values to use for the list.
      template <typename T>
      List(std::vector<T> list) {
        m_object = to_pyobject(list);
      }

      // If it's not a list of PyObjects or Items, then it shouldn't be
      // something that needs to track a PyObject reference to as it will be
      // creating new PyObjects.
      /* */
      /// @brief Create an Item for a Python List object from an initializer
      ///        list of some generic type.
      /// @tparam <T> The type that will be converted to Python objects for
      ///             the list (should be a type that can be made into a
      ///             Python object).
      /// @param list The list of values to use for the list.
      template <typename T>
      List(std::initializer_list<T> list) {
        m_object = to_pyobject(list);
      }

      /// @brief Returns the number of elements stored in the list.
      /// @return The size of the Python list.
      size_t size() const { return PyList_Size(m_object); }

      // NOTE: PyList_Append increments the refcount of the item itself.
      /* */
      /// @brief Adds the given item at the end of the List object.
      /// @tparam <T> The type of item to append to the list. Can be a
      ///             PyObject, pxx Item type, or other type that can be
      ///             made into a PyObject.
      /// @param item The item to put into the list.
      /// @return Whether or not the item was added successfully.
      template <typename T>
      bool append(T item) {

        // 0 means successful insertion.
        return PyList_Append(m_object, to_pyobject(item)) == 0;
      }

      // NOTE: insert increments the refcount of the item itself.
      /* */
      /// @brief Adds the given item at the start of the List object.
      /// @tparam <T> The type of item to prepend to the list. Can be a
      ///             PyObject, pxx Item type, or other type that can be
      ///             made into a PyObject.
      /// @param item The item to put into the list.
      /// @return Whether or not the item was added successfully.
      template <typename T>
      bool prepend(T item) {
        return this->insert(0, item);
      }

      // NOTE: PyList_Insert increments the refcount of the item itself.
      /* */
      /// @brief Inserts the given item in front of the item at the given
      ///        index in the list.
      /// @tparam <T> The type of item to insert into the list. Can be a
      ///             PyObject, pxx Item type, or other type that can be
      ///             made into a PyObject.
      /// @param index Where to insert the item in front of; can be negative
      ///              like in Python to denote index from end of list.
      /// @param item The item to put into the list.
      /// @return Whether or not the item was inserted successfully.
      template <typename T>
      bool insert(long long index, T item) {
        // Allow for negative indices by converting to positive.
        if (index < 0) { index = this->size() + index; }

        // -1 means exception for insertion.
        return PyList_Insert(m_object, index, to_pyobject(item)) != -1;
      }

      // Returns a new Item(), so new reference.
      /* */
      /// @brief Get the item at the given index in the list.
      /// @param index The index of the item to get; can be negative like in
      ///              Python to denote index from end of list.
      /// @return A new Item with the retrieved Python object; the underlying
      ///         PyObject will be NULL if index is out of bounds.
      Item get(long long index) const {
        // Allow for negative indices by converting to positive.
        if (index < 0) { index = this->size() + index; }

        // PyList_GetItem returns a borrowed reference, but Item() will
        // create a strong reference for it automatically.
        return Item(PyList_GetItem(m_object, index));
      }

      // Returns a new Item(), so new reference.
      /* */
      /// @brief Get the item at the given index in the list.
      /// @param index The index of the item to get; can be negative like in
      ///              Python to denote index from end of list.
      /// @return A new Item with the retrieved Python object; the underlying
      ///         PyObject will be NULL if index is out of bounds.
      Item operator[](long long index) const {
        return get(index);
      }

      /// @brief Returns the first item in the list.
      /// @return A new Item with the retrieved Python object; if there is no
      ///         first item, the underlying PyObject will be NULL.
      Item first() const {
        // get() creates new reference.
        return this->get(0);
      }

      /// @brief Returns everything but the first element of the list.
      /// @return A new List object with everything but the first element; if
      ///         unsuccessful, the underlying PyObject for the new slice will
      ///         be NULL.
      List rest() const {
        // slice() creates new reference.
        return this->slice(1);
      }

      /// @brief Returns the last item in the list.
      /// @return A new Item with the retrieved Python object; if there is no
      ///         last item, the underlying PyObject will be NULL.
      Item last() const {
        // get() creates new reference.
        return this->get(this->size() - 1);
      }

      /// @brief Puts the given item into the spot at the given index in the
      ///        list, removing any item that was already there.
      /// @param index The index to put the new item.
      /// @param item The item to insert into the list.
      /// @return Whether the item was inserted successfully or not.
      bool set(long long index, Item item) {
        // Allow for negative indices by converting to positive.
        if (index < 0) { index = this->size() + index; }

        // Copy reference for item to be removed so we can deref after.
        PyObject* temp = PyList_GetItem(m_object, index);

        // -1 means out of bounds exception.
        // PyList_SetItem steals reference to inserted item, so this needs
        // to be countered.
        bool result = PyList_SetItem(m_object, index, item.to_object()) != -1;

        // Deref removed item.
        Py_XDECREF(temp);

        // Add reference to added item.
        item.add_reference();

        return result;
      }

      /// @brief Puts the given item into the spot at the given index in the
      ///        list, removing any item that was already there.
      /// @param index The index to put the new item.
      /// @param item The item to insert into the list.
      /// @return Whether the item was inserted successfully or not.
      bool set(long long index, PyObject* item) {
        // Allow for negative indices by converting to positive.
        if (index < 0) { index = this->size() + index; }

        // Copy reference for item to be removed so we can deref after.
        PyObject* temp = PyList_GetItem(m_object, index);

        // -1 means out of bounds exception.
        // PyList_SetItem steals reference to inserted item, so this needs to
        // be countered.
        bool result = PyList_SetItem(m_object, index, item) != -1;

        // Deref removed item.
        Py_XDECREF(temp);

        // Add reference to added item.
        Py_XINCREF(item);

        return result;
      }

      /// @brief Puts the given item into the spot at the given index in the
      ///        list, removing any item that was already there.
      /// @tparam <T> The type of item to insert into the list. Needs to be
      ///             a type that can be converted to a PyObject.
      /// @param index The index to put the new item.
      /// @param item The item to insert into the list.
      /// @return Whether the item was inserted successfully or not.
      template <typename T>
      bool set(long long index, T item) {
        // Allow for negative indices by converting to positive.
        if (index < 0) { index = this->size() + index; }

        // Copy reference for item to be removed so we can deref after.
        PyObject* temp = PyList_GetItem(m_object, index);

        // -1 means out of bounds exception.
        // PyList_SetItem steals reference to inserted item, but this is fine
        // since the given item is a newly made PyObject.
        bool result = PyList_SetItem(m_object, index, to_pyobject(item)) != -1;

        // Deref removed item.
        Py_XDECREF(temp);

        return result;
      }

      // Analogous to list[low:high]
      // Note that slicing is [low, high) access.
      /* */
      /// @brief Returns a new List of objects between the start and end
      ///        indexes. Note that the slice is [start, end) like in Python,
      ///        not [start, end].
      /// @param start The index to start the slice at.
      /// @param end The index to end the slice at.
      /// @return A new List object containing the slice of the current list.
      ///         The underlying PyObject will be NULL if getting the slice
      ///         was unsuccessful.
      List slice(long long start, long long end) const {
        // Allow for negative indices by converting to positive.
        if (start < 0) { start = this->size() - start; }
        if (end < 0) { end = this->size() - end; }

        // Guard clause; short-circuit return empty list.
        if (this->size() == 0 || (start > end)) { return List(); }

        // PyList_GetSlice returns new reference.
        PyObject* newList = PyList_GetSlice(m_object, start, end);

        // List() adds a reference, so we need to remove it so we don't
        // double-reference item.
        auto result = List(newList);
        result.remove_reference();

        return result;
      }

      // slice() will return a new reference.
      /* */
      /// @brief Returns a new List of objects that is everything in the
      ///        list from the given index.
      /// @param start The index to start the slice at.
      /// @return A new List object containing the slice of the current list.
      ///         The underlying PyObject will be NULL if getting the slice
      ///         was unsuccessful.
      List slice(long long start) const {
        return this->slice(start, this->size());
      }

      // TODO: Add duplicate to Item class; creates copy of object in a manner
      //       that updates all references as if an entirely new item was made.
      /* */
      /// @brief Creates a new copy of the list (not just a reference to the
      ///        list, but a new list).
      /// @return A new List with all the same items.
      List duplicate() const {
        // This should work and make new references; test when possible.
        return this->slice(0, this->size());
      }

      /// @brief Sorts the list in-place.
      /// @return Whether the sorting was successful or not.
      bool sort() {
        // -1 means sort failure.
        return PyList_Sort(m_object) != -1;
      }

      /// @brief Returns a sorted version of the list; this is a new list, not
      ///        a reference to the current list.
      /// @return A new List object that is a sorted version of the list.
      List sorted() const {
        // duplicate() makes new reference.
        List newList = this->duplicate();
        newList.sort();

        return newList;
      }

      /// @brief Reverses the list in-place.
      /// @return Whether the reversal was successful or not.
      bool reverse() {
        // -1 means reversal failure.
        return PyList_Reverse(m_object) != -1;
      }

      /// @brief Returns a reversed version of the list; this is a new list,
      ///        not a reference to the current list.
      /// @return A new List object that is a reversed version of the list.
      List reversed() const {
        // duplicate() makes new reference.
        List newList = this->duplicate();
        newList.reverse();

        return newList;
      }

      /// @brief Removes the item at the end of the list and returns it.
      /// @return The popped item at the end of the list, the underlying
      ///         PyObject will be NULL if nothing to pop.
      Item pop() {
        // Guard clause: Just return blank item if nothing to pop.
        if (this->size() < 1) { return Item(); }

        Item lastItem = this->last();
        List newList = this->slice(0, this->size() - 1);

        // Remove reference to old list object.
        this->remove_reference();

        // Add reference to new list object as the List will self-dereference
        // at end of function.
        m_object = newList.to_object();
        this->add_reference();

        return lastItem;
      }

      /// @brief Removes the item at the given index and returns it.
      /// @return The popped item at the given index, the underlying
      ///         PyObject will be NULL if nothing to pop.
      Item pop(long long index) {
        // Guard clause: Just return blank item if nothing to pop.
        if (this->size() < 1) { return Item(); }

        Item popped = this->get(index);
        PyObject* newList = PyList_New(this->size() - 1);

        for (int x = 0; x < this->size(); x++) {
          // Skip the item that is supposed to be popped.
          if (x == index) { continue; }

          // PyList_SET_ITEM will steal the new reference from
          // PyList_GetItem, so the new reference is not leaked.
          PyList_SET_ITEM(newList, x, PyList_GetItem(m_object, x));
        }

        // Remove reference to old list; this will remove reference to
        // popped item when old list is dead.
        this->remove_reference();
        m_object = newList;

        return popped;
      }

      std::vector<Item> to_vector() const {
        std::vector<Item> list;

        for (int x = 0; x < this->size(); x++) {
          list.push_back(this->get(x));
        }

        return list;
      }

      Iterator begin() {
        return Iterator(this, 0);
      }

      const Iterator begin() const {
        return Iterator(this, 0);
      }

      Iterator end() {
        return Iterator(this, this->size());
      }

      const Iterator end() const {
        return Iterator(this, this->size());
      }

      // NOTE: PyList_AsTuple does not need to be implemented as
      //       pxx::Tuple(list) will do this.
  };
}

#endif