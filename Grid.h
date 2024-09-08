// rlpm -*- C++ -*-
// Rory McGuire cs351 spr03 Project 2

#ifndef GRID_H
#define GRID_H

// include system headers
#include <stddef.h>   // for size_t

// Forward declare what will be in each location
class GridAble;

// The Grid class
class Grid {

  friend class World; // so we can dump the grid

public:

  // A direction enum
  typedef enum {
    NORTH=0,
    SOUTH,
    EAST,
    WEST
  } direction;

  Grid(size_t x=DEF_COLS, size_t y=DEF_ROWS); // ctor
  ~Grid(); // dtor

  // Place a GridAble in a specific location
  bool Place(GridAble *gable, size_t x, size_t y);

  // Place a GridAble in a random location
  bool PlaceRandom(GridAble *gable);

  // Place a GridAble next to another one
  bool PlaceNextTo(GridAble *old, direction dir, GridAble *gable);

  // Move a GridAble a specified direction
  bool Move(GridAble *gable, direction dir);

  // Clear the GridAble from this Grid
  bool Clear(GridAble *gable);

  // Clear the whole Grid
  void ClearAll();

  // Get the total number of GridAbles in the grid
  size_t Occupants() const;

  // whether or not a direction from a GridAbleis a border
  bool isBorder(GridAble *gable, direction dir) const;

  // Get the neighbor of a GridAble
  GridAble* GetNeighbor(GridAble *gable, direction dir);

private:
  // define away shallow copies
  Grid(const Grid &);
  void operator=(const Grid &);

  // Get a GridAble in a location
  GridAble* Peek(size_t x, size_t y) const;

  // whether or not a location is a border
  bool isBorder(size_t x, size_t y) const;

  // add to or subtract from abscissa or coordinate
  void DirLoc(size_t &x, size_t &y, direction dir) const;

  // default size of the grid
  const static size_t DEF_COLS = 60;
  const static size_t DEF_ROWS = 30;

  // The number of GridAbles in the grid
  size_t _occupants;

  // The max x and y values (used to determine boundaries)
  size_t _max_x;
  size_t _max_y;

  // The internal array
  GridAble ***_array;

};

#endif // GRID_H
