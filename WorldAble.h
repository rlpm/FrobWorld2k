// rlpm -*- C++ -*-
// Rory McGuire cs351 spr03 Project 2

#ifndef WORLDABLE_H
#define WORLDABLE_H

// forward declarations
class World;

// The WorldAble base class
class WorldAble {

  friend class World; // so world can modify us

public:

  virtual ~WorldAble(); // dtor

protected:

  // Make this class somewhat abstract by defining constructor as protected
  // so it can't be instantiated directly.
  // Always require a World, unlike PQable and GridAble, which may be in
  // 'limbo' -- not in the PQueue or Grid.
  WorldAble(World *w); // default ctor

  // Get the World
  World* GetWorld() const;

private:

  // don't allow shallow copies
  WorldAble(const WorldAble &);
  void operator=(const WorldAble &);

  // The grid we're in
  World *_w;

};

#endif // WORLDABLE_H
