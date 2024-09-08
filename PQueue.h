// rlpm -*- C++ -*-
// Rory McGuire cs351 spr03 Project 2

#ifndef PQUEUE_H
#define PQUEUE_H

// include system headers
#include <stddef.h>  // for size_t
#include <vector>

class PQable;

// The main priority queue class
class PQueue {
public:

  PQueue();     // default ctor
  ~PQueue();    // dtor

  // Push a value into the queue
  void Push(PQable*);

  // Pop the next highest priority value
  PQable* Pop();

  // Peek at the next highest priority
  size_t Peek();

  // Remove an entry from the queue
  bool Delete(PQable*);

  // The number of values in the queue
  const size_t Size() const;

  void dump() const; // for testing

private:

  // don't allow shallow copies
  PQueue(const PQueue &);
  void operator=(const PQueue &);

  // Upheap beginning at index
  void upheap(size_t);

  // Downheap beginning at index
  void downheap(size_t);

  // Swap two nodes
  void swap(size_t,size_t);

  // The heap array
  std::vector<PQable*> _array;
};

#endif // PQUEUE_H
