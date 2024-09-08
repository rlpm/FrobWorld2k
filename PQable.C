// rlpm -*- C++ -*-
// Rory McGuire cs351 spr03 Project 2

// include system headers
#include <stddef.h>   // for size_t

// include project includes
#include "PQable.h"
#include "PQueue.h"

// ctor
// pass everything to base initializers
PQable::PQable() : _pq(NULL), _pq_loc(0), _pq_priority(0) {}

// dtor
// make sure to remove it from the PQueue
PQable::~PQable() {
  if (_pq) _pq->Delete(this);
}

// Get the PQ
PQueue* PQable::GetPQ() const {
  return _pq;
}

// Set the PQ
void PQable::SetPQ(PQueue* pq) {
  _pq = pq;
}

// Get the location in the PQ
size_t PQable::GetPQLoc() const {
  return _pq_loc;
}

// Set the location in the PQ
void PQable::SetPQLoc(size_t loc) {
  _pq_loc = loc;
}

// Get the Priority
size_t PQable::GetPriority() const {
  return _pq_priority;
}

// Set the Priority
void PQable::SetPriority(size_t p) {
  _pq_priority = p;
}
  
