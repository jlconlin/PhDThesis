# $Author: jlconlin $
# $Id: Makefile 323 2008-06-27 19:06:16Z jlconlin $
# $Revision: 323 $
# $Date: 2008-06-27 13:06:16 -0600 (Fri, 27 Jun 2008) $
 
# Modeled after the paper "Recursive Make Considered Harmful"
#  http://aegis.sourceforge.net/auug97.pdf,
# and after the example in the book
# Managaing Projects with GNU Make 3rd ed. pg. 117

modules := Particle Material RandomLib Geometry Exception FissionSource \
	LinearAlgebra Misc test

programs :=
sources :=
libraries :=
extra_clean := 


objects :=
programs_objs :=
dependencies = $(subst .cpp,.d, $(subst .cc,.d, $(sources)))
include_dirs := . boost/ boost-sandbox/

# Look for include files in each of the modules
#.INCLUDE_DIRS = /usr/include /usr/local/include /usr/include
INCLUDES = $(addprefix -I , $(include_dirs) /usr/include /usr/local/include /usr/include)
COMP_FLAGS = -O3 -funroll-loops 
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
LAPACK := -framework Accelerate

all:

# Include the description for each module
include $(patsubst %, %/module.mk, $(modules)) 

.PHONY: all
all: $(programs)
	$(warning program $@)

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

