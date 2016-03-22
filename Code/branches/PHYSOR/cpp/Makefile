# $Author: jlconlin $
# $Id: Makefile 397 2008-10-13 21:06:07Z jlconlin $
# $Revision: 397 $
# $Date: 2008-10-13 15:06:07 -0600 (Mon, 13 Oct 2008) $
 
# Modeled after the paper "Recursive Make Considered Harmful"
#  http://aegis.sourceforge.net/auug97.pdf,
# and after the example in the book
# Managaing Projects with GNU Make 3rd ed. pg. 117

modules := Particle Material RandomLib Geometry Exception FissionSource \
	Markov LinearAlgebra Misc Gnuplot Research Research/ParametricStudies test

programs :=
sources :=
libraries :=
extra_clean := 


objects :=
programs_objs :=
dependencies = $(subst .cpp,.d, $(subst .cc,.d, $(sources)))
include_dirs := . ~/src/boost_1_34_1/

# Look for include files in each of the modules
#.INCLUDE_DIRS = /usr/include /usr/local/include /usr/include
INCLUDES = $(addprefix -I , $(include_dirs) /usr/include /usr/local/include /usr/include)
COMP_FLAGS = -O3 -funroll-loops -DNITERCHK
LINK_FLAGS := 
# CPPFLAGS += $(addprefix -I , $(include_dirs)) -g -pg
vpath %.h %.hh $(addsuffix /, $(include_dirs) ./)

CC := g++
MV := mv -f
RM := rm -f
SED := sed

# By redefining these commands, I can specify in one location everything that 
# happens during compilation and linking as well as add extra flags 
COMPILE.d = $(CXX) $(CXXFLAGS) $(CPPFLAGS) $(TARGET_ARCH) $(INCLUDES) $(COMP_FLAGS)
COMPILE.cc = $(COMPILE.d) -c
LINK.cc = $(CXX) $(CXXFLAGS) $(CPPFLAGS) $(LDFLAGS) $(TARGET_ARCH) $(LINK_FLAGS)

all:

# Include the description for each module
include $(patsubst %, %/module.mk, $(modules)) 

.PHONY: all
all: $(programs)
	$(warning program $@)

#$(warning objects $(objects))
#$(warning dependencies $(dependencies))

# Include the C++ include dependencies
ifneq "$(MAKECMDGOALS)" "clean"
  include $(dependencies)
endif

# Calculate C++ include dependencies
%.d: %.cc
	$(COMPILE.d) -M $< | \
	$(SED) 's,\($(notdir $*)\.o\) *:,$(dir $@)\1 $@: ,' > $@.tmp
	$(MV) $@.tmp $@

%.d: %.cpp 
	$(COMPILE.d) -M $< | \
	$(SED) 's,\($(notdir $*)\.o\) *:,$(dir $@)\1 $@: ,' > $@.tmp
	$(MV) $@.tmp $@


clean:
	$(warning INCLUDES $(INCLUDES))
	$(RM) $(objects) $(programs) $(libraries)  \
	      $(dependencies) $(extra_clean) $(programs_objects)

#$(warning objects $(objects))
#$(warning programs $(programs))
#$(warning dependencies $(dependencies))
#$(warning extra_clean $(extra_clean))
#$(warning programs_objects $(programs_objects))
