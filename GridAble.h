// rlpm -*- C++ -*-
// Rory McGuire cs351 spr03 Project 2

#ifndef GRIDABLE_H
#define GRIDABLE_H

// include system headers
#include <stddef.h>   // for size_t

// forward declarations
class Grid;

// The GridAble base class
class GridAble {

  friend class Grid; // so it can muck around inside this base class
                     // (but not derived classes)

public:

  virtual ~GridAble(); // dtor

  // Get the location
  bool GetLoc(size_t &x, size_t &y) const;

protected:

  // Make this class somewhat abstract by defining constructor as protected
  // so it can't be instantiated directly.
  GridAble(); // default ctor

  // Get the grid
  Grid* GetGrid() const;

private:

  // don't allow shallow copies
  GridAble(const GridAble &);
  void operator=(const GridAble &);

  // Set the Grid we're currently in
  void SetGrid(Grid *g);

  // Set the Location
  bool SetLoc(size_t x, size_t y);

  // The grid we're in
  Grid *_g;

  // The location
  size_t _x;
  size_t _y;

};

#endif // GRIDABLE_H
