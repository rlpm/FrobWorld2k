// rlpm -*- C++ -*-
// Rory McGuire cs351 spr03 Project 2

// include system headers
#include <stdlib.h>   // for random
#include <assert.h>

// include project includes
#include "Grid.h"
#include "GridAble.h"

// main ctor
// create arrays of arrays
Grid::Grid(const size_t x, const size_t y) :
  _occupants(0), _max_x(x), _max_y(y) {
  _array = new GridAble**[x];
  for(size_t i=0;i<x;i++) {
    _array[i] = new GridAble*[y];
    for(size_t j=0;j<y;j++)
      _array[i][j] = NULL;
  }
}

// dtor
// clear occupants if they exist
Grid::~Grid() {
  for(size_t i=0;i<_max_x;i++) {
    // Set each occupant's grid to NULL
    if (_occupants) {
      for(size_t j=0;j<_max_y;j++) {
        if (_array[i][j]) Clear(_array[i][j]);
      }
    }
    delete [] _array[i];
  }
  delete [] _array;
}

// Get the GridAble in a location
GridAble* Grid::Peek(size_t x, size_t y) const {
  // make sure we're not over the boundaries
  if (x>_max_x-1 || y>_max_y-1) return NULL;
  return _array[x][y];
}

// Place a (new) GridAble in a location
bool Grid::Place(GridAble *gable, size_t x, size_t y) {
  // make sure it's not already in a grid
  if (gable->GetGrid()) return false;

  // make sure we're not on or over a boundary
  if (!x || !y || x>=_max_x-1 || y>=_max_y-1) return false;

  // make sure there's not already something there
  if(_array[x][y]) return false;

  _array[x][y] = gable;
  gable->SetGrid(this);
  gable->SetLoc(x,y);
  _occupants++;
  return true;
}
  
// Move a GridAble based on direction
bool Grid::Move(GridAble *gable, direction dir) {
  // make sure it's in this grid
  if (gable->GetGrid() != this) return false;

  size_t x,y;
  gable->GetLoc(x,y);
  size_t new_x = x, new_y = y;
  DirLoc(new_x,new_y,dir);

  // make sure it's not a border
  if (isBorder(new_x,new_y)) return false;

  // make sure it's empty
  if (Peek(new_x,new_y)) return false;

  // move the bugger
  _array[x][y] = NULL;
  _array[new_x][new_y] = gable;
  gable->SetLoc(new_x,new_y);
  return true;
}

// Place a GridAble next to another one
bool Grid::PlaceNextTo(GridAble *old, direction dir, GridAble *gable) {
  // make sure it's in this grid
  if (old->GetGrid() != this) return false;

  size_t x,y;
  old->GetLoc(x,y);
  DirLoc(x,y,dir);

  // make sure it's not a border
  if (isBorder(x,y)) return false;

  // make sure it's empty
  if (Peek(x,y)) return false;

  // place the bugger
  _array[x][y] = gable;
  gable->SetGrid(this);
  gable->SetLoc(x,y);
  _occupants++;
  return true;
}

// Place a GridAble in a random location
bool Grid::PlaceRandom(GridAble *c) {
  // determine maximum number of creatures in the grid, so below isn't
  // an infinite loop
  if (_max_x < 2 && _max_y < 2) return false;
  size_t maximum = (_max_x-2) * (_max_y-2);
  if (Occupants() == maximum) return false;

  while (!Place(c, 1 + (random() % (_max_x - 2)),
                1 + (random() % (_max_y - 2))));
  return true;
}

// Clear a GridAble from a location
bool Grid::Clear(GridAble *gable) {
  if (gable->GetGrid() != this) return false;

  size_t x,y;
  if (!gable->GetLoc(x,y)) return false;

  // if we blow this assertion, someone else somehow set our location!!!
  assert (_array[x][y] == gable);

  gable->SetGrid(NULL);
  _array[x][y] = NULL;
  _occupants--;
  return true;
}

// Clear the whole Grid
void Grid::ClearAll() {
  for(size_t x=0;x<_max_x;x++) {
    for(size_t y=0;y<_max_y;y++) {
      if (_array[x][y])
        _array[x][y]->SetGrid(NULL);
      _array[x][y] = NULL;
    }
  }
}

// return the number of occupants
size_t Grid::Occupants() const {
  return _occupants;
}

// whether or not the location in a direction is a border
bool Grid::isBorder(GridAble *gable, direction dir) const {
  size_t x=0,y=0;

  // get this GridAble's location
  bool ingrid = gable->GetLoc(x,y);
  // make sure it's in the grid
  assert (ingrid);

  // get new location
  DirLoc(x,y,dir);

  return isBorder(x,y);
}

// whether or not a location is a border
bool Grid::isBorder(size_t x, size_t y) const {
  if (!x || !y || x==_max_x-1 || y==_max_y-1) return true;
  return false;
}

// Get a neighbor based on GridAble and direction
GridAble* Grid::GetNeighbor(GridAble *gable, direction dir) {
  size_t x=0,y=0;

  // get this GridAble's location
  bool ingrid = gable->GetLoc(x,y);
  // make sure it's in the grid
  assert (ingrid);

  // get the new location
  DirLoc(x,y,dir);

  return Peek(x,y);
}

// add to or subtract from abscissa or coordinate, based on direction
void Grid::DirLoc(size_t &x, size_t &y, direction dir) const {
  if (dir == NORTH) {
    assert(y);
    y--;
  } else if (dir == SOUTH) {
    assert(y < _max_y - 1);
    y++;
  } else if (dir == EAST) {
    assert(x < _max_x - 1);
    x++;
  } else {
    assert(x);
    x--;
  }
  return;
}
