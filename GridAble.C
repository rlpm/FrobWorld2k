// rlpm -*- C++ -*-
// Rory McGuire cs351 spr03 Project 2

#include "GridAble.h"
#include "Grid.h"

// ctor
GridAble::GridAble() : _g(NULL), _x(0), _y(0) {}

// dtor
GridAble::~GridAble() {
  if (_g) _g->Clear(this);
}

// Get the location
Grid* GridAble::GetGrid() const {
  return _g;
}

// Set the Grid
void GridAble::SetGrid(Grid *g) {
  _g = g;
}
  
// Get the location
bool GridAble::GetLoc(size_t &x, size_t &y) const {
  if (!_g) return false;
  x = _x;
  y = _y;
  return true;
}

// Set the Location
bool GridAble::SetLoc(size_t x, size_t y) {
  if (!_g) return false;
  _x = x;
  _y = y;
  return true;
}
