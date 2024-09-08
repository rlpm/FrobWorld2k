// rlpm -*- C++ -*-
// Rory McGuire cs351 spr03 Project 2

// include system headers
#include <stddef.h>   // for size_t
#include <iostream> // for ostream

// include project includes
#include "Creature.h"

// ctor
// just base to base initializers
Creature::Creature(World *w, ostream &o) : PQable(), GridAble(),
                                           WorldAble(w),
                                           _mass(0), _updatevalue(0),
                                           _out(o) {}

// dtor
// nothing to do - taken care of by base and derived classes
Creature::~Creature() {}

// Get the mass
size_t Creature::GetMass() const {
  return _mass;
}

// Set the mass
void Creature::SetMass(size_t m) {
  _mass = m;
}

// Get the UpdateVal (virtual, may be overridden)
size_t Creature::GetUpdateVal() const {
  return _updatevalue;
}

// Set the UpdateVal
// if GetUpdateVal is overridden, this may not do much
void Creature::SetUpdateVal(size_t u) {
  _updatevalue = u;
}

// output the "new" event for this creature
// call the (virtual) OutputVals method
void Creature::OutputNew() {
  GetO() << "new { ";
  OutputVals();
  GetO() << "}" << endl;
}

// output the "kill" event for this creature
// call the (virtual) OutputVals method
void Creature::OutputKill() {
  GetO() << "kill { ";
  OutputVals();
  GetO() << "}" << endl;
}

// output the "move" event for this creature
// must already know the old and new locations
// call the (virtual) OutputVals method
void Creature::OutputMove(size_t x, size_t y, size_t nx, size_t ny) {
  GetO() << "move { " << x << " " << y << " ";
  GetO() << nx << " " << ny << " }" << endl;
}

// output the piece of the "world" event for this creature
// call the (virtual) OutputVals method
void Creature::OutputDump() {
  GetO() << "( ";
  OutputVals();
  GetO() << ")";
}

// return the output stream on which to output stuff
ostream& Creature::GetO() {
  return _out;
}
