# $Author: jlconlin $
# $Id: Makefile 236 2008-01-03 21:14:32Z jlconlin $
# $Revision: 236 $
# $Date: 2008-01-03 14:14:32 -0700 (Thu, 03 Jan 2008) $
 
# Modeled after the paper "Recursive Make Considered Harmful"
#  http://aegis.sourceforge.net/auug97.pdf,
# and after the example in the book
# Managaing Projects with GNU Make 3rd ed. pg. 117

modules := Particle Material RandomLib Geometry Exception FissionSource \
	Markov Misc test

programs :=
sources :=
libraries :=
extra_clean := 


objects :=
programs_objs :=
dependencies = $(subst .cpp,.d, $(subst .cc,.d, $(sources)))
include_dirs := . ~/src/boost_1_34_1/

# Look for include files in each of the modules
CPPFLAGS += $(addprefix -I , $(include_dirs)) -O3 -funroll-loops 
# CPPFLAGS += $(addprefix -I , $(include_dirs)) -g -pg
vpath %.h %.hh $(addsuffix /, $(include_dirs) ./)

CC := g++
MV := mv -f
RM := rm -f
SED := sed

all:

# Include the description for each module
include $(patsubst %, %/module.mk, $(modules)) 

.PHONY: all
all: $(programs)

#$(warning objects $(objects))
#$(warning dependencies $(dependencies))

# Include the C++ include dependencies
ifneq "$(MAKECMDGOALS)" "clean"
  include $(dependencies)
endif

%.cpp %.hpp:

# Calculate C++ include dependencies
%.d: %.cc
	$(CC) $(CFLAGS) $(CPPFLAGS) $(TARGET_ARCH) -M $< | \
	$(SED) 's,\($(notdir $*)\.o\) *:,$(dir $@)\1 $@: ,' > $@.tmp
	$(MV) $@.tmp $@

%.d: %.cpp 
	$(CC) $(CFLAGS) $(CPPFLAGS) $(TARGET_ARCH) -M $< | \
	$(SED) 's,\($(notdir $*)\.o\) *:,$(dir $@)\1 $@: ,' > $@.tmp
	$(MV) $@.tmp $@


clean:
	$(warning sources $(sources))
	$(RM) $(objects) $(programs) $(libraries)  \
	      $(dependencies) $(extra_clean) $(programs_objects)

#$(warning objects $(objects))
#$(warning programs $(programs))
#$(warning dependencies $(dependencies))
#$(warning extra_clean $(extra_clean))
#$(warning programs_objects $(programs_objects))
