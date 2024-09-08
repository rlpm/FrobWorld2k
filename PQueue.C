// rlpm -*- C++ -*-
// Rory McGuire cs351 spr03 Project 2

#include <assert.h>

// for testing
#include <iostream>
#include <iomanip>

#include "PQueue.h"
#include "PQable.h"

// default ctor - init unused 0th element in array
PQueue::PQueue() {
  _array.push_back(NULL);
}

// dtor
// make sure the current occupants think they're removed
PQueue::~PQueue() {
  for (size_t i=1;i<_array.size();i++) {
    _array[i]->SetPQ(NULL);
    _array[i]->SetPQLoc(0);
  }
}
    
// Push a value into the queue
void PQueue::Push(PQable* p) {
  assert (!p->GetPQ());    // must not already be in a PQ
  _array.push_back(p);     // push it onto the top of the heap
  p->SetPQ(this);          // set the pq intrusively
  p->SetPQLoc(Size());     // set the location intrusively
  upheap(Size());          // bubble bottom up the heap
}  

// Pop the value off the top of the heap array
PQable* PQueue::Pop() {
  if (!Size())              // nothing to pop
    return NULL;

  PQable *ret = _array[1];  // save the head
  ret->SetPQ(NULL);         // set its intrusive stuff
  ret->SetPQLoc(0);

  if (Size() == 1) {        // just deleted last node
    _array.resize(_array.size()-1);
    return ret;
  }

  _array[1] = _array[_array.size()-1];  // pull last node to top
  _array[1]->_pq_loc = 1;               // set the location intrusively
  _array.resize(_array.size()-1);       // downsize
  downheap(1);                          // bubble top down the heap

  return ret;
}

// Peek at the next highest priority
size_t PQueue::Peek() {
  assert(Size());
  return _array[1]->GetPriority();
}

// Delete an entry based on value
bool PQueue::Delete(PQable *p) {
  if (!Size()) return false;        // if there's no entries, return false
  if (p->GetPQ() != this)           // not in this PQ
    return false;

  size_t loc = p->GetPQLoc();       // get the loc stored in the object

  if (loc == 0) return false;       // not in the PQueue

  size_t old_p = p->GetPriority();  // remember old priority
  p->SetPQ(NULL);                   // set intrusive PQ pointer
  p->SetPQLoc(0);                   // set intrusive PQ location

  if (loc == _array.size()-1) {     // deleting last node
    _array.resize(_array.size()-1);
    return true;
  }

  _array[loc] = _array[_array.size()-1];
  _array[loc]->SetPQLoc(loc);       // set intrusive PQ location
  _array.resize(_array.size()-1);

  if (_array[loc]->GetPriority() > old_p)
    downheap(loc);
  else
    upheap(loc);

  return false;
}

// The number of values in the queue
const size_t PQueue::Size() const {
  return _array.size() - 1;
}

// bubble up the heap
void PQueue::upheap(size_t idx) {
  while(idx != 1) {
    size_t prt = idx/2;  // get the parent index

    // if higher priority than the parent, swap
    if (_array[idx]->GetPriority() < _array[prt]->GetPriority()) {
      swap(idx,prt);
      idx = prt;
    } else {
      break;
    }
  }
}

// bubble down the heap
void PQueue::downheap(size_t idx) {
  size_t last=Size();

  while(idx < last) {
    size_t kid = idx*2;

    // no left child (and no right child), so we're done
    if (kid > last)
      break;

    // see if right child is higher prio than left
    if (kid+1 <= last &&
        _array[kid+1]->GetPriority() < _array[kid]->GetPriority())
      kid++;

    // see if we should swap with this kid
    if (_array[idx]->GetPriority() > _array[kid]->GetPriority()) {
      swap(kid,idx);
      idx = kid;
    } else {
      break;
    }
  } // while
}

// swap two locations in the array
void PQueue::swap(size_t a, size_t b) {
  size_t s = _array.size();
  assert(a && a < s);
  assert(b && b < s);
  PQable *tmp = _array[a];
  _array[a] = _array[b];
  _array[b] = tmp;
  _array[a]->SetPQLoc(a);
  _array[b]->SetPQLoc(b);
}

void PQueue::dump() const {
  cout << "Size: " << Size() << endl;
  for (size_t i=1;i<_array.size();i++) {
    cout << setw(3) << i << ":" << setw(3) << _array[i]->GetPQLoc() << " "
         << setw(3) << _array[i]->GetPriority()
         << " " << _array[i] << endl;
  }
  cout << "---------------------" << endl;
}
