#pragma once

#include <exception>

enum Color {  // Represents a list color.
  Red = 0,
  Blue = 1,
};

class InvalidIndexException : public std::exception {};

template <typename T>
class DoubleList {
 public:
  class Node  // Represents a node of the double list.
  {
    friend class DoubleList;

   private:
    T mValue;        // Value of the node.
    Node *mNext[2];  // A next pointer for each color.
   public:
    // Getter for the value.
    T value() const { return mValue; }
    // Getter for the next pointers.
    Node *next(Color color) const { return mNext[color]; }
  };

 private:
  Node *mHead[2];  // A head pointer for each color.

 public:
  // Creates an empty double list.
  DoubleList() : mHead{nullptr, nullptr} {}
  // Creates double list from given array.
  DoubleList(T *a, unsigned n);
  // Clones the given double list.
  DoubleList(const DoubleList &dl);
  // Returns the pointer to the node in 'color' list at index.
  Node *get(unsigned index, Color color) const;
  // Inserts a new node with value to both lists at corresponding indices.
  Node *insert(T value, unsigned redIndex, unsigned blueIndex);
  // Remves the node at index of 'color' list.
  void remove(unsigned index, Color color);
  // Appends the contents of given dl into this double list. Empties dl.
  void append(DoubleList &dl);
  // Destroys the double list.
  ~DoubleList();

  // Getter for the head pointers.
  Node *head(Color color) const { return mHead[color]; }
};