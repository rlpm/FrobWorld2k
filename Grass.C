// rlpm -*- C++ -*-
// Rory McGuire cs351 spr03 Project 2

// include system headers
#include <stddef.h>   // for size_t
#include <stdlib.h>   // for random
#include <iostream> // for ostream
#include <assert.h>
#include <vector>

#include "Grass.h"
#include "Frob.h"
#include "Grid.h"
#include "PQueue.h"
#include "World.h"

// setup static floats, since ANSI C++ warns us if we do it in the
// class declaration
const float Grass::BIRTHFRACTION = 0.4;
const float Grass::MASSFACTOR = .20;

// ctor
// set the updateval, and randomize the priority and mass
Grass::Grass(World *w, ostream &o) : Creature(w,o) {
  SetUpdateVal(INITIALUPDATEINTERVAL);
  SetMass(random()%GENESISMASS);
  SetPriority(random()%GetUpdateVal());
}

// dtor
// nothing to do -- done by base classes
Grass::~Grass() {}

// output some stuff about yourself
void Grass::OutputVals() {
  GetO() << "grass ";
  size_t x=0,y=0;
  if (GetLoc(x,y))
    GetO() << x << " " << y;
  else
    GetO() << "NULL NULL";
  GetO() << " " << GetMass() << " " << GetUpdateVal() << " ";
}

// live a litte, possibly trying to birth a new grass
bool Grass::TurnToLive(bool display) {
  SetMass(GetMass()+(GetUpdateVal()*MASSFACTOR));

  while (GetMass()>=BIRTHMASS) {
    if (!TryBirth(display))
      break;
  }

  // so we can reschedule for a later time
  SetPriority(GetPriority()+GetUpdateVal());
  return true;
}

// try to birth a grass
bool Grass::TryBirth(bool display) {
  size_t buddies = 0;
  vector<Grid::direction> open; // keep track of open locations
  grok tmp;

  // look around to see what's there
  // we could do this in a for loop, since directions are an enum,
  // but just in case they aren't...
  tmp = Look(Grid::NORTH);
  if (tmp == GRASS)
    buddies++;
  if (tmp == EMPTY)
    open.push_back(Grid::NORTH);

  tmp = Look(Grid::SOUTH);
  if (tmp == GRASS)
    buddies++;
  if (tmp == EMPTY)
    open.push_back(Grid::SOUTH);

  tmp = Look(Grid::EAST);
  if (tmp == GRASS)
    buddies++;
  if (tmp == EMPTY)
    open.push_back(Grid::EAST);

  tmp = Look(Grid::WEST);
  if (tmp == GRASS)
    buddies++;
  if (tmp == EMPTY)
    open.push_back(Grid::WEST);

  // if we're under the crowd limit, randomly choose one of the empty
  // locations in which to birth a new grass
  if (buddies < CROWDLIMIT && open.size()) {
    Grid::direction dir=open[random()%open.size()];
    Grass *kid = new Grass(GetWorld());
    kid->SetMass(GetMass()*BIRTHFRACTION);
    SetMass(GetMass()-kid->GetMass());
    kid->SetPriority(GetPriority()+1 + (random()%kid->GetUpdateVal()));

    // can't just get this Grass's PQ, since it's been popped out
    GetWorld()->GetPQ()->Push(kid);

    // this Grass is still in the Grid, so we can use GetGrid()
    GetGrid()->PlaceNextTo(this,dir,kid);
    if (display)
      kid->OutputNew();
    return true;
  } else {
    // didn't qualify to birth, so get lazier
    SetMass(BIRTHMASS);
    SetUpdateVal(GetUpdateVal()*2);
    if (GetUpdateVal()>MAXUPDATEINTERVAL)
      SetUpdateVal(MAXUPDATEINTERVAL);
    return false;
  }
}

// what it understands (groks) about its neighbors
Grass::grok Grass::Look(Grid::direction dir) {
  if (GetGrid()->isBorder(this,dir))
    return OTHER;

  GridAble *gable = GetGrid()->GetNeighbor(this,dir);
  if (!gable)
    return EMPTY;
    
  Creature *c = dynamic_cast<Creature*>(gable);
  assert(c); // this would be VERY bad!

  if (dynamic_cast<Grass*>(c))
    return GRASS;
  else if (dynamic_cast<Frob*>(c))
    return OTHER;

  return OTHER;
}
