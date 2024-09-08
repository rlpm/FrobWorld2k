// rlpm -*- C++ -*-
// Rory McGuire cs351 spr03 Project 2

#include <iostream>
#include <stddef.h>
#include <stdlib.h>
#include <time.h>
#include <string>
#include <cmath> // for sqrt - for stddev

#include "World.h"
#include "PQueue.h"
#include "Creature.h"
#include "Grass.h"
#include "Frob.h"
#include "Grid.h"

// ctor
// pass values to base initializers, then GO!
World::World(istream &i, ostream &o, ostream &e):_date(0),_interval(1),
                                                 _g(NULL),_pq(NULL),
                                                 _frobs(0),
                                                 _in(i),_out(o),_err(e),
                                                 _dbgout(0) {
  Go();
}

// dtor
// clear the world, to free memory
World::~World() {
  Clear();
}

// dump the grid (for testing)
// on my machine, "\001" is a smiley face. :)
void World::DumpGrid() {
  if (!_g) return;
  size_t x,y,i,j;
  Creature *c;
  x = _g->_max_x;
  y = _g->_max_y;
  _out << " ";
  for (j=0;j<x;j++) _out << j%10;
  _out << endl;
  for (i=0;i<y;i++) {
    _out << i%10;
    for (j=0;j<x;j++) {
      if ((c = dynamic_cast<Grass*>(_g->Peek(j,i)))) _out << "#";
      else if ((c = dynamic_cast<Frob*>(_g->Peek(j,i)))) _out << "\001";
      else _out << " ";
    }
    _out << endl;
  }
}

// delete PQueue and Grid, and free memory of objects created
void World::Clear() {
  if (_g)  {
    _g->ClearAll();
    delete _g;
    _g = NULL;
  }
  if (_pq) {
    PQable *p;
    while((p = _pq->Pop()))
      delete p;
    delete _pq;
    _pq = NULL;
  }
  _date = 0;
}

// initialize the world, clearing first (so no memory leaks!)
bool World::Init(size_t cols, size_t rows, size_t seed, size_t frobs,
          size_t grasses) {
  Clear();

  // create a new Grid and PQueue
  _g = new Grid(cols,rows);
  _pq = new PQueue();

  // seed the PRNG
  if (!seed) seed = time(NULL);
  srandom(seed);

  // send the reset event
  OutputReset(cols, rows, seed, frobs, grasses);

  // create the creatures, sending new events
  for (size_t i=frobs+grasses;i;i--) {
    Creature *c;
    if (i>frobs) c = new Grass(this);
    else c = new Frob(this);
    _pq->Push(c);
    if (!_g->PlaceRandom(c)) {
      _err << "Too many Creatures!!!" << endl;
      Clear();
      return false;
    }
    c->OutputNew();
  }

  // send the initial clock event
  OutputClock();
  return true;
}

// send the reset event
void World::OutputReset(size_t cols, size_t rows, size_t seed, size_t frobs,
                        size_t grasses) {
  _out << "reset { " << cols << " " << rows << " " << seed << " ";
  _out << frobs << " " << grasses << " }" << endl;
}

// send the clock event
void World::OutputClock() {
  _out << "clock { " << _date << " }" << endl;
}

// send the extinct event
// the first value is the clock at the time of extinction
// the second value is the number of frobs still living
// the (optional) third value is the mean of their metabolic rates
// (update intervals)
// the (optional) fourth value is the Standard Deviation of the
// metabolic rates
void World::OutputExtinct() {
  _out << "extinct { " << _date << " " << TotalFrobs() << " ";

  if (!_g || !TotalFrobs()) {
    _out << "}" << endl;
    return;
  }

  // compute average metabolic rate (update interval) for frobs

  // first, dump all values into a vector
  size_t i, j, x=_g->_max_x, y=_g->_max_y;
  vector<size_t> vals;
  for (i=0;i<y;i++) {
    for (j=0;j<x;j++) {
      Frob *f;
      if ((f = dynamic_cast<Frob*>(_g->Peek(j,i))))
        vals.push_back(f->GetUpdateVal());
    }
  }

  // compute the total
  size_t total=0;
  for (i=0;i<vals.size();i++)
    total += vals[i];

  // compute the average
  double avg=static_cast<double>(total)/vals.size();

  // compute the variant sum
  double variant_sum=0;
  for (i=0;i<vals.size();i++)
    variant_sum += (vals[i]-avg) * (vals[i]-avg);

  // compute the stddev
  double stddev=sqrt(variant_sum/vals.size());

  // send them to the output stream
  _out << avg << " " << stddev << " }" << endl;
}

// send the world event (dumping the clock and all the creatures)
void World::OutputWorld() {
  if (!_g) return;
  size_t x,y,i,j;
  Creature *c;
  x = _g->_max_x;
  y = _g->_max_y;
  _out << "world { " << _date << " ";
  for (i=0;i<y;i++) {
    for (j=0;j<x;j++) {
      if ((c = dynamic_cast<Creature*>(_g->Peek(j,i)))) {
        c->OutputDump();
        _out << " ";
      }
    }
  }
  _out << "}" << endl;
}

// read input commands and act accordingly 
// note the extra undocumented commands "rlpm" and "grid", which
// toggle the debug output (dumps everything that's happening), and
// outputs the grid, respectively
void World::Go() {
  while (_in.good()) {
    string cmd;
    size_t args[5];
    _in >> cmd;
    if (cmd == "init") { // init command from spec
      for(int i=0;i<5;i++)
        _in >> args[i];
      if (!Init(args[0],args[1],args[2],args[3],args[4])) {
        _err << "Init failed" << endl;
      }
    } else if (cmd == "step") { // step command from spec
      for(int i=0;i<2;i++)
        _in >> args[i];
      if (args[1] && args[1]!=1)
        _err << "Illegal input" << endl;
      else
        Step(args[0],args[1]);
    } else if (cmd == "quit") { // quit command from spec
      break;
    } else if (cmd == "interval") { // interval command from spec
      _in >> args[0];
      if (!args[0])
        _err << "Illegal input" << endl;
      else
        _interval = args[0];
    } else if (cmd == "rlpm") { // toggle debug output
      _dbgout = !_dbgout;
    } else if (cmd == "grid") { // dump the grid
      DumpGrid();
    } else if (cmd == "dump") { // dump command from spec
      OutputWorld();
    } else {
      if (_in.eof())
        break;
      _err << "Unknown command" << endl;
    }
  }
}    

// step through the world a specified number of times, possibly
// displaying new kill and move events
void World::Step(size_t steps, bool display) {
  for (;steps;steps--) {
    // make sure to stop at the right date
    size_t enddate = _date + _interval;
    if (enddate > MAXSIMULATIONLENGTH)
      enddate = MAXSIMULATIONLENGTH;

    while(TotalFrobs() && (_pq->Size())) {
      // Make sure the next guy in the PQ is in this step. If not, and
      // we pop it, even if we push it back again, the PQ doesn't
      // guarantee order, e.g. changing the interval might change
      // output, even if the random seed is the same!!!
      if (_pq->Peek()>enddate)
        break;

      Creature *next=dynamic_cast<Creature*>(_pq->Pop());
      if (!next) // something BAD happened
        continue;

      // set the date
      _date = next->GetPriority();

      // let the creature live a little
      bool stayalive = next->TurnToLive(display);
      if (!stayalive) {
        // it died :(
        if (display)
          next->OutputKill();
        delete next;
      } else {
        // reschedule it
        _pq->Push(next);
      }
    }

    // debug output
    if (_dbgout)
      system("clear");

    // check to see if we went extinct
    if (!TotalFrobs() || _date == MAXSIMULATIONLENGTH) {
      OutputExtinct();
      // debug output
      if (_dbgout)
        DumpGrid();
      break;
    } else {
      // set clock to appropriate end date for this step
      _date = enddate;
      // send the clock event
      OutputClock();
      // debug output
      if (_dbgout) {
        DumpGrid();
        _out << "Frobs: " << TotalFrobs() << endl;
      }
    }
  }
}
