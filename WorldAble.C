// rlpm -*- C++ -*-
// Rory McGuire cs351 spr03 Project 2

#include "WorldAble.h"
#include "World.h"

// ctor
WorldAble::WorldAble(World *w) : _w(w) {}

// dtor
WorldAble::~WorldAble() {}

// Get the World
World* WorldAble::GetWorld() const {
  return _w;
}
