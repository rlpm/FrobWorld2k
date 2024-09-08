// rlpm -*- C++ -*-
// Rory McGuire cs351 spr03 Project 2

#ifndef PQABLE_H
#define PQABLE_H

// include system headers
#include <stddef.h>   // for size_t

class PQueue;

// The PQable base class
class PQable {

  friend class PQueue; // So that PQueues can modify us

public:

  // be able to destruct (especially for subclasses)
  virtual ~PQable(); // dtor

  // Get the priority
  // Needs to be public to step clock
  size_t GetPriority() const;

protected:

  // protect ctor to make class somewhat abstract
  // can't instantiate directly
  PQable(); // default ctor

  // Get which PQueue this object is in
  PQueue* GetPQ() const;

  // Set the Priority
  void SetPriority(size_t p);

private:

  // don't allow shallow copies
  PQable(const PQable &);
  PQable& operator=(const PQable &);

  // Set which PQueue this object is in
  void SetPQ(PQueue *pq);

  // Get the Location in the PQueue
  // this is only allowed to friend PQueue and this class
  size_t GetPQLoc() const;

  // Set the Location in the PQueue
  // this is only allowed to friend PQueue and this class
  void SetPQLoc(size_t loc);

  // It would be nice for friends not to be able to get to data members,
  // but access control is not that granular

  // The PQueue this object is in
  PQueue *_pq;

  // The location in the PQueue
  size_t _pq_loc;

  // The priority
  size_t _pq_priority;

};

#endif // PQABLE_H
