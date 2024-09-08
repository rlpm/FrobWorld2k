// rlpm -*- C++ -*-
// Rory McGuire cs351 spr03 Project 2

#ifndef CREATURE_H
#define CREATURE_H

// include system headers
#include <stddef.h>   // for size_t
#include <iostream> // for ostream

#include "PQable.h"
#include "GridAble.h"
#include "WorldAble.h"

// forward declarations
class World;

// The Creature base class
class Creature : public PQable, public GridAble, public WorldAble {
public:

  Creature(World *w, ostream &o=cout); // ctor
  virtual ~Creature(); // dtor

  // Get the mass
  size_t GetMass() const;

  // Get the UpdateVal
  virtual size_t GetUpdateVal() const;

  // Live a little
  virtual bool TurnToLive(bool display) = 0;

  // Output a 'new' event
  void OutputNew();

  // Output a 'kill' event
  void OutputKill();

  // Output a 'move' event
  void OutputMove(size_t x, size_t y, size_t nx, size_t ny);

  // Output a piece for a 'dump' event
  void OutputDump();

protected:

  // Print some stuff about yourself
  virtual void OutputVals() = 0;

  // Set the mass
  void SetMass(size_t);

  // Set the mass
  void SetUpdateVal(size_t);

  // Get the output stream
  ostream& GetO();

private:

  // don't allow shallow copies
  Creature(const Creature &);
  void operator=(const Creature &);

  // The Mass
  size_t _mass;

  // The update value
  size_t _updatevalue;

  // output stream to print stuff to
  ostream &_out;

};

#endif // CREATURE_H
