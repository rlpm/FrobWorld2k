// rlpm -*- C++ -*-
// Rory McGuire cs351 spr03 Project 2

#ifndef GRASS_H
#define GRASS_H

// include system headers
#include <stddef.h>   // for size_t
#include <iostream.h> // for ostream

#include "Creature.h"
#include "Grid.h"

// The Grass class
class Grass: public Creature {
public:

  Grass(World *w, ostream &o=cout); // ctor
  virtual ~Grass(); // dtor

  // Live a little
  virtual bool TurnToLive(bool display);

  // Print some stuff about yourself
  virtual void OutputVals();

private:

  // don't allow shallow copies
  Grass(const Grass &);
  void operator=(const Grass &);

  // What a frob understands (groks)
  // The word "grok" comes from Robert Heinlein's Book
  // "Stranger in a Strange Land"
  typedef enum {
    EMPTY=0, // nothing in neighboring spot
    GRASS=1, // grass in neighboring spot
    OTHER=2  // other in neighboring spot (unavailable, but not grass)
  } grok;

  // What is in the grid in that direction
  grok Look(Grid::direction dir);

  // Try to (and possibly) birth a new grass
  bool TryBirth(bool display);

  // Fraction of mass given to offspring
  static const float BIRTHFRACTION;

  // Mass at which Grasses wish to split
  static const size_t BIRTHMASS = 30;

  // 4-neighborhood Grass count for no splitting
  static const size_t CROWDLIMIT = 2;

  // Grass fixed mass cost per action
  static const size_t FIXEDOVERHEAD = 0;

  // Initial Grass mass
  static const size_t GENESISMASS = 10;

  // Initial Days between Grass activities
  static const size_t INITIALUPDATEINTERVAL = 10;

  // Grass mass gain per day
  static const float MASSFACTOR;

  // Max inactive days on failed splitting
  static const size_t MAXUPDATEINTERVAL = 100;

};

#endif // GRASS_H
