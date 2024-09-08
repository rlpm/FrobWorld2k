             CS351 Project 2 - frobworld:2k

CONTACT INFORMATION
 Project author:  Rory L.P. McGuire
 Email:           rlpm@unm.edu
 Date:            Mon Mar 31 2003 05:40:47 MST (GMT-0700)

PROJECT DESCRIPTION

 frobworld:2K (aka fw2k) is a program to explore individual efficiency
 and ecological stability using discrete event simulation.  See
 http://cs.unm.edu/~ackley/cs351/projects/2 for a complete
 description.

 Shipping manifest: 
  Contained in the distribution you will find the following files:
   Creature.[Ch]  - A base class to help all types of simulated entities
                  + Derives from PQable, GridAble and WorldAble
   Frob.[Ch]      - A class to simulate Frob entities
   Grass.[Ch]     - A class to simulate Grass entities
   GridAble.[Ch]  - A base class for the intrusive Grid container
   Grid.[Ch]      - A class that holds a set of locations and the objects
                  + in those locations
                  + Requires that entries be derived from GridAble
   PQable.[Ch]    - A base class for the intrusive PQueue container
   PQueue.[Ch]    - A heap-based priority queue
                  + Requires that entries be derived from PQable
   WorldAble.[Ch] - A base class for the intrusive world container
   World.[Ch]     - The World class declaration
                  + This is the high level wrapper for the other classes
   main.[Ch]      - Simple main function and NOOP header file
   Makefile       - The (GNU) Makefile for building fw2k
                  + note that the Makefile will build the make.depend file
   README.txt     - This file
   results.txt    - The data file supporting my answers to questions posed
                  + in s1.1 of the spec.

 How to build:
  (1) Unpack the tar file into an empty directory
  (2) cd to that directory
  (3) Type 'make' to build fw2k
      Please note: this will complain about non-existant file
      make.depend, which is a dependency list.  That file will be
      automatically generated, and make will continue properly.
      See Makefile for more information.

 How to run:
  fw2k accepts no command-line arguments.

  fw2k reads from standard input and writes to standard output, so you
  can supply input from the keyboard and/or redirect input and/or
  output as you wish.  See the spec for commands.

ACKNOWLEDGMENTS
               
 Professor Ackley designed the spec as well as providing a nifty GUI
 and a sample implementation.  He also provided the basis for the
 PQueue and PQable classes.  I discussed dynamic casting and virtual
 functions with Prof Ackley, Keith Wiley, and Ryan McNeal.
 Prof. Ackley provided some insights and his opinions on dynamic
 casting vs. virtual functions.  I decided to use dynamic casting,
 since I had to downcast from the intrusive container-able base
 classes, or put stuff into the base classes that they really didn't
 need to know about.

 Thanks to Bjarne Stroustrup for C++; the Free Software Foundation for
 the GNU Project, in particular g++, GNU make and Debian GNU/Linux;
 The XEmacs group; AT&T, Bell Labs, and others for Unix; RedHat/Cygnus
 for cygwin; Linus Torvalds for Linux; Julian Seward for valgrind; and
 Robert Heinlein for "grok".
               
KNOWN BUGS & UNDOCUMENTED FEATURES

 - Nothing major.
 - After a successful completion, it has still reachable memory
   allocated by stl_alloc.h, but that's a issue in the STL and not
   my code.
 - If given bad input, sometimes it will exit, and sometimes it will
   continue with unspecified results.  This adheres to the spec,
   which states that behavior in these cases is undefined.
 - Issuing a 'grid' command (no params) will print the grid, with '#'
   for a Grass, and '\001' (smiley face on my machine) for a Frob.
 - Issuing an 'rlpm' command (no params) will toggle 'debug' mode,
   where a great deal of information is dumped to the output stream.

IMPLEMENTATION DETAILS

 I have several base classes used by the intrusive containers PQueue,
 Grid, and World.  Creature inherits multiply from all three, and
 then Grass and Frob inherit from Creature. 

 The Frob class contains a nested DNA class with several parameters
 from the spec.

 The Frob and Grass classes contain parameters from the spec, minus
 the "FROB" or "GRASS" prefix, since they were put into their
 respective classes.  These classes also contain enums for what is
 understandable to each type of creature.  For instance, Frobs can
 tell the difference between Rocks, Frobs, Grasses and Empty
 locations in their neighborhood.  Grasses can only distiguish
 other Grasses, Empty locations, and Other (Frob or Rock).  The name
 given to these enums is 'grok', from Robert Heinlein's book
 "Stranger in a Strange Land".

 The Grid class contains a typedef'd enum to distinguish the four
 directions, North, South, East, and West.

ANSWERS TO QUESTIONS POSED BY THE SPEC:

 To answer the questions, I did not use Tkfw2k (since I can't seem to
 get my internal network to forward port 6000 to my laptop for X
 windows), I parsed through the Tkfw2k script to determine that it
 initializes 50 frobs and 50 grasses for its simulations.  So, I used
 the following input, over 50 runs, making sure to have at least 1
 second between runs, so that my random seeds were all different:

  init 60 30 0 50 50
  interval 100000
  step 1 0

 Of those 50 simulations, 15 ran to 100000 days with Frobs remaining.
 In those 15 simulations, the average metabolic rate (UpdateInterval)
 of the frobs was:

   #Frobs  Mean      StdDev
   ------  -------   ---------
     124   49.0726    6.692770
      61   30.0492    1.683340
      47    6.0000    0.000000
      89   23.2921    5.035160
      55   26.3455    3.004350
      46   31.5652   14.032000
      30   16.3333    5.962850
     175   33.2857    0.990568
      38   19.7895   10.247400
     107   18.7944    6.195780
      52   40.5385   15.133300
     113   17.1150    0.784273
      83   20.1084    9.111110
      58   20.3448    4.508810
     181   70.4254    2.136560

 The supporting data (reset and extinct events only) is in the file
 named "results.txt".
