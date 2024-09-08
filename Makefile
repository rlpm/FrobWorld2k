# rlpm -*- Makefile -*-
# Rory McGuire cs351 spr03 Project 2

########################################################################
# This is the Makefile used to build fw2k
########################################################################

# Use these flags for all compilations
CXXFLAGS=-Wall -pedantic -g

# The program to build
PROG=fw2k

# The sources
SRCS=PQable.C PQueue.C GridAble.C Grid.C Creature.C Grass.C \
     Frob.C WorldAble.C World.C main.C

# The objects
OBJS=$(SRCS:%.C=%.o)

# Even using cygwin, GNU make, and g++, my WinXP box adds .exe to
# executable filenames.  So, do a funny GNU make dance to fix that
# brain damage.
OS=$(shell uname)
ifeq "$(OS)" "CYGWIN_NT-5.1"
# Include the executable filename extension for distclean target
EXT=.exe
endif
# end of Windows stuff

# Set the program name with extension
PRG=$(PROG)$(EXT)

# The all target
all: $(PRG)

# the main target
$(PRG): $(OBJS)
	$(CXX) $^ -o $@

# clean targets
clean:
	$(RM) $(OBJS) depend.make

distclean: clean
	$(RM) $(PRG)

# make the dependency lists
depend.make:  Makefile $(SRCS) $(SRCS:%.C=%.h)
	$(CXX) -E -MM $(CPPFLAGS) $(SRCS) > depend.make
depend: depend.make

# DO NOT DELETE THE FOLLOWING LINE
include depend.make
