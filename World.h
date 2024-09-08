// rlpm -*- C++ -*-
// Rory McGuire cs351 spr03 Project 2

#ifndef WORLD_H
#define WORLD_H

// includes
#include <stddef.h> // for size_t
#include <iostream> // for istream,ostream

// forward declarations
class Grid;
class PQueue;

// The World class
class World {
public:

  World(std::istream &i=std::cin, std::ostream &o=std::cout, std::ostream &e=std::cerr); // ctor
  ~World(); // dtor

  // needed to birth creatures, since they're not in the PQ when they
  // get their turn to live
  PQueue* GetPQ() {return _pq;};

  // Keep track of total number of living frobs
  size_t TotalFrobs() const {return _frobs;};
  void IncFrobs() {_frobs++;};
  void DecFrobs() {_frobs--;};

private:
  // define away shallow copies
  World(const World &);
  void operator=(const World &);

  // Clear the PQ and Grid
  void Clear();

  // Run the simulator
  void Go();

  // Initialize a World
  bool Init(size_t cols, size_t rows, size_t seed, size_t frobs,
            size_t grasses);

  // Step through the world
  void Step(size_t steps, bool display);

  // Various output functions
  void OutputReset(size_t cols, size_t rows, size_t seed, size_t frobs,
                   size_t grasses);
  void OutputClock();
  void OutputExtinct();
  void OutputWorld();

  // Dump the grid out to the output stream (for testing)
  void DumpGrid();

  // constants
  static const size_t MAXSIMULATIONLENGTH = 100000;

  // The internal variables
  size_t _date;
  size_t _interval;

  Grid *_g;
  PQueue *_pq;

  size_t _frobs;

  std::istream &_in;
  std::ostream &_out;
  std::ostream &_err;

  bool _dbgout; // for testing

};

#endif // WORLD_H
