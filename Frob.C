// rlpm -*- C++ -*-
// Rory McGuire cs351 spr03 Project 2

// include system headers
#include <stddef.h>   // for size_t
#include <stdlib.h>   // for random
#include <iostream>   // for ostream
#include <assert.h>

#include "Frob.h"
#include "Grid.h"
#include "Grass.h"
#include "World.h"
#include "PQueue.h"
#include "Grid.h"

// setup static floats, since ANSI C++ warns us if we do it in the
// class declaration
const float Frob::MASSTAX = .15;

// ctor
// setup the mass and genome, set a priority from the genome,
// and increment the world's total frob count
Frob::Frob(World *w, ostream &o) : Creature(w,o) {
  SetMass(GENESISMASS);
  _genome.Randomize();
  SetPriority(random()%(GetUpdateVal()+1));
  GetWorld()->IncFrobs();
}

// birthing ctor
// create this frob as a child of the parent frob, placing it into the
// grid, mutating the genome, reducing the parent's mass, scheduling
// it into the PQueue, and incrementing the World's total frob count.
Frob::Frob(Frob &parent, size_t x, size_t y, World *w, ostream &o)
  : Creature(w,o) {
  parent.GetGrid()->Place(this,x,y);
  _genome = parent._genome;
  _genome.Mutate();
  SetMass(parent._genome.BirthFrac()*parent.GetMass());
  parent.SetMass(parent.GetMass()-GetMass());
  SetPriority(parent.GetPriority()+(random()%(GetUpdateVal()+1)));
  parent.GetWorld()->GetPQ()->Push(this);
  GetWorld()->IncFrobs();
}

// dtor
Frob::~Frob() {
  // just decrement the World's total frob count
  GetWorld()->DecFrobs();
}

size_t Frob::GetUpdateVal() const {
  // use the genome rather than Creature data member
  return _genome.UpdateInterval();
}

// output some stuff about this frob
void Frob::OutputVals() {
  GetO() << "frob ";
  size_t x,y;
  if (GetLoc(x,y))
    GetO() << x << " " << y;
  else
    GetO() << "NULL NULL";
  GetO() << " " << GetMass() << " ";
  _genome.Output(GetO());
}

// live a litte
bool Frob::TurnToLive(bool display) {
  // determine mass reduction
  size_t massredux = ((size_t)(MASSTAX * GetUpdateVal())) + FIXEDOVERHEAD;

  // if mass reduction will kill it, set mass to 0 and return false
  if (massredux >= GetMass()) {
    SetMass(0);
    return false;
  }

  // reduce the mass
  SetMass(GetMass()-massredux);

  // look around at neighborhood and choose where it wants to go
  size_t gonorth, gosouth, goeast, gowest;

  gonorth=LikeToMove(Grid::NORTH);
  gosouth=gonorth+LikeToMove(Grid::SOUTH);
  goeast=gosouth+LikeToMove(Grid::EAST);
  gowest=goeast+LikeToMove(Grid::WEST);

  size_t whichdir = random()%gowest;
  Grid::direction dir=Grid::WEST;

  if (whichdir < gonorth) {
    dir = Grid::NORTH;
  } else if (whichdir < gosouth) {
    dir = Grid::SOUTH;
  } else if (whichdir < goeast) {
    dir = Grid::EAST;
  }

  // try going that direction
  if (!Go(dir, display))
    return false;

  // add to the priority for rescheduling
  SetPriority(GetPriority() + GetUpdateVal());
  return true;
}

// how much it likes to move in a direction, based on what's there and
// genome values
size_t Frob::LikeToMove(Grid::direction dir) {
  size_t baseprefs = DNA::NORTHPREFS;
  if (dir == Grid::SOUTH) baseprefs = DNA::SOUTHPREFS;
  else if (dir == Grid::EAST) baseprefs = DNA::EASTPREFS;
  else if (dir == Grid::WEST) baseprefs = DNA::WESTPREFS;

  return _genome[baseprefs + Look(dir)];
}

// what it understands (groks) about its neighbors
Frob::grok Frob::Look(Grid::direction dir) {
  if (GetGrid()->isBorder(this,dir))
    return ROCK;

  GridAble *gable = GetGrid()->GetNeighbor(this,dir);
  if (!gable)
    return EMPTY;
    
  Creature *c = dynamic_cast<Creature*>(gable);
  assert(c);
  if (dynamic_cast<Grass*>(c))
    return GRASS;
  else if (dynamic_cast<Frob*>(c))
    return FROB;

  // really bad things happen if we get here
  assert(0);
  return EMPTY;
}

// try and move in a direction
bool Frob::Go(Grid::direction dir, bool display) {
  // determine what is in that direction
  grok who=Look(dir);

  // call the appropriate method(s) depending on what's there
  if (who == ROCK) {
    return BumpRock();
  } else if (who == EMPTY) {
    Move(dir,display);
    return true;
  } else {
    GridAble *gable = GetGrid()->GetNeighbor(this,dir);
    if (who == FROB) {
      FightFrob(dynamic_cast<Frob*>(gable),display);
      return true;
    } else if (who == GRASS) {
      EatGrass(dynamic_cast<Grass*>(gable),display);
      // determine if it should procreate
      if (GetMass()==_genome.BirthMass()) {
        size_t x,y;
        // save the old location to drop the kid into
        GetLoc(x,y);
        Move(dir,display);
        Frob *kid = new Frob(*this,x,y,GetWorld());
        if (display)
          kid->OutputNew();
      } else {
        Move(dir,display);
      }
      return true;
    }
  }
  // Don't know what's going on, so by default, die
  assert(0);
  return false;
}

// Move a specific direction
void Frob::Move(Grid::direction dir, bool display) {
  size_t x, y;
  if (display)
    GetLoc(x,y);
  if (GetGrid()->Move(this,dir)) {
    if (display) {
      size_t nx, ny;
      GetLoc(nx,ny);
      OutputMove(x,y,nx,ny);
    }
  } else {
    // something bad happened if we got here and tried to move to an
    // invalid grid location
    assert(0);
  }
}

// bump a rock, subtract from mass, possibly die
bool Frob::BumpRock() {
  if (ROCKBUMPPENALTY < GetMass()) {
    SetMass(GetMass()-ROCKBUMPPENALTY);
    return true;
  }
  SetMass(0);
  return false;
}

// fight another frob, possibly killing it
void Frob::FightFrob(Frob *enemy, bool display) {
  if (enemy->GetMass() > HITPENALTY) {
    enemy->SetMass(enemy->GetMass()-HITPENALTY);
  } else {
    enemy->SetMass(0);
    if (display)
      enemy->OutputKill();
    delete enemy;
  }
}

// eat some yummy grass, kill it, and possibly decrease this frob's
// mass to genome(BirthMass)
void Frob::EatGrass(Grass *yummy, bool display) {
  if (display)
    yummy->OutputKill();
  SetMass(GetMass()+yummy->GetMass());
  delete yummy;
  if (GetMass() > _genome.BirthMass())
    SetMass(_genome.BirthMass());
}

// randomize the genome
void Frob::DNA::Randomize() {
  for (int i=0;i<LEN;i++)
    _data[i] = random()&0xff;
}

// (possibly) mutate the genome
void Frob::DNA::Mutate() {
  for (int i=0;i<LEN;i++) {
    if (!(random()%MUTATIONODDSPERBYTE)) {
      unsigned char flipper = 0x1 << (random()&0xf);
      _data[i] ^= flipper;
    }
  } 
}

// output the genome
void Frob::DNA::Output(ostream &o) {
  for (int i=0;i<LEN;i++)
    o << static_cast<int>(_data[i]) << " ";
}
