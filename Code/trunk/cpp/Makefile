# $Author: jlconlin $
# $Id: Makefile 771 2009-08-20 02:17:36Z jlconlin $
# $Revision: 771 $
# $Date: 2009-08-19 20:17:36 -0600 (Wed, 19 Aug 2009) $
 
# Modeled after the paper "Recursive Make Considered Harmful"
#  http://aegis.sourceforge.net/auug97.pdf,
# and after the example in the book
# Managaing Projects with GNU Make 3rd ed. pg. 117

modules := Particle Material RandomLib Geometry Exception FissionSource \
	LinearAlgebra Misc test Markov Research # Benchmark

programs :=
sources := 
libraries :=
extra_clean := 


objects :=
gnuplot_objs = 
programs_objs :=
dependencies = $(subst .cpp,.d, $(subst .cc,.d, $(sources)))
include_dirs := . boost-sandbox/ gnuplot-cpp/trunk/ \
	/trunk/usr/include /usr/local/include 

# Look for include files in each of the modules
#.INCLUDE_DIRS = /usr/include /usr/local/include /usr/include
INCLUDES = $(addprefix -I , $(include_dirs) )
COMP_FLAGS = -DNFIELDCHK 
COMP_FLAGS += -DNITERCHK 
COMP_FLAGS += -funroll-loops 
COMP_FLAGS += -O3 
COMP_FLAGS += -DNMESHCONNECTIVITYCHECK
COMP_FLAGS += -DNINPUTCH
COMP_FLAGS += -DNDEBUG
COMP_FLAGS += -DNOASSERT

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
LAPACK := -framework Accelerate		# Mac OS X
# LAPACK := -L/usr/local/lib -L/usr/lib/gcc/i386-redhat-linux/3.4.3/ -lg2c -llapack -lblas -lm  # Linux

all:

# Include the description for each module
include $(patsubst %, %/module.mk, $(modules)) 

.PHONY: all
all: $(programs)

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
	$(warning objects: $(objects))
	$(RM) $(objects) $(programs) $(libraries)  \
	      $(dependencies) $(extra_clean) $(programs_objects)

