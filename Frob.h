// rlpm -*- C++ -*-
// Rory McGuire cs351 spr03 Project 2

#ifndef FROB_H
#define FROB_H

// include system headers
#include <stddef.h>   // for size_t
#include <iostream> // for ostream

// subclass headers
#include "Creature.h" // base class
#include "Grid.h"     // for Grid::direction

// forward declarations
class Grass;

// The Frob class
class Frob: public Creature {
public:

  Frob(World *w, std::ostream &o=std::cout); // ctor
  virtual ~Frob(); // dtor

  // Get the UpdateVal
  // Will be overridden to use DNA genome, rather than Creature data
  // member
  virtual size_t GetUpdateVal() const;

  // Live a little
  virtual bool TurnToLive(bool display);

  // Print some stuff about yourself
  virtual void OutputVals();

private:

  // don't allow shallow copies
  Frob(const Frob &);
  void operator=(const Frob &);

  // build a child frob, but don't want a copy constructor
  Frob(Frob &parent, size_t x, size_t y, World *w, std::ostream &o=std::cout);

  // How much this Frob likes to move in a direction depending on
  // what's there
  size_t LikeToMove(Grid::direction dir);

  // Try to go a certain direction
  // returns false if this Frob dies
  bool Go(Grid::direction dir, bool display);

  // Actually Move a specific direction
  void Move(Grid::direction dir, bool display);

  // Bump a rock, incur some damage
  // returns false if this frob dies
  bool BumpRock();

  // Eat some (yummy) grass
  void EatGrass(Grass *yummy, bool display);

  // Fight (and possibly kill) another Frob
  void FightFrob(Frob *enemy, bool display);

  // What a frob understands (groks)
  // The word "grok" comes from Robert Heinlein's Book
  // "Stranger in a Strange Land"
  typedef enum {
    EMPTY=0, // neighbor is empty
    ROCK=1,  // neighbor is rock
    GRASS=2, // neighbor is grass
    FROB=3   // neighbor is frob
  } grok;

  // What is in the grid in that direction
  grok Look(Grid::direction dir);

  // DNA values
  class DNA {
  public:
    enum {           // Names of dna fields
      BIRTHMASS=0,   // dna[0] controls birthmass (s10.2.2.1)
      BIRTHFRAC=1,   // dna[1] controls birthfrac (s10.2.2.2)
      UPDINTERVAL=2, // dna[2] controls updinterval (s10.2.2.3)
      NORTHPREFS=3,  // dna[3..6] controls north prefs (s10.3.2)
      SOUTHPREFS=7,  // base index for south preferences (s10.3.2)
      EASTPREFS=11,  //  ditto, east (s10.3.2)
      WESTPREFS=15,  //  ditto, west (s10.3.2)
      LEN=19         // OVERALL LENGTH OF DNA
    };

    // get various genome-related values
    size_t BirthMass() const {return _data[BIRTHMASS]+20;};
    float BirthFrac() const {return _data[BIRTHFRAC]/255.0*.8+.1;};
    size_t UpdateInterval() const {return _data[UPDINTERVAL]/2+1;};

    // return something useful
    unsigned char operator[](int idx) const {return _data[idx];};

    // randomize the dna
    void Randomize();

    // Mutate the DNA a bit
    void Mutate();

    // Output the values
    void Output(std::ostream &);

  private:

    // 1-in-this chance of a bit flip per DNA byte
    static const size_t MUTATIONODDSPERBYTE = 20;

    // the actual DNA data
    unsigned char _data[LEN];

  } _genome;  // the instance of DNA needed by each Frob

  // Frob fixed mass cost per action
  static const size_t FIXEDOVERHEAD = 1;

  // Initial Frob mass
  static const size_t GENESISMASS = 100;

  // Mass penalty when Frob hits Frob
  static const size_t HITPENALTY = 10;

  // Frob mass loss per day (see Frob.C for definition)
  static const float MASSTAX;

  // Frob mass loss if it hits a rock
  static const size_t ROCKBUMPPENALTY = 30;

};

#endif // FROB_H
