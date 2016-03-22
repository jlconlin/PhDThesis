# $Author: jlconlin $
# $Id: module.mk 547 2009-05-16 01:23:43Z jlconlin $
# $Revision: 547 $
# $Date: 2009-05-15 19:23:43 -0600 (Fri, 15 May 2009) $
 
local_dir := Markov
local_files := PowerTest.cpp ArnoldiMCTest.cpp
local_src := $(addprefix $(local_dir)/,$(local_files))

# arnoldiMC_src := $(addprefix $(local_dir)/,ArnoldiMC.cpp)
# arnoldiMC_objs := $(subst .cpp,.o,$(arnoldiMC_src))
# arnoldiMC_objs += 

local_tests := PowerTest ArnoldiMCTest
local_pgm := $(addprefix $(local_dir)/,$(local_tests))
local_test_src := $(addsuffix .cpp,$(local_pgm))
local_test_objs := $(addsuffix .o,$(local_pgm))

local_objs := $(arnoldiMC_objs)

programs += $(local_pgm)
modules += $(local_dir)
sources += $(local_src) $(local_test_src)
include_dirs += $(local_dir)
objects += $(local_objs) $(local_test_objs)

$(arnoldiMC_objs): $(local_src) 

$(local_dir)/ArnoldiMCTest: $(local_dir)/ArnoldiMCTest.o \
	$(random_objs) $(particle_objs) $(mesh_objs) $(hist_objs) $(linspace_objs)
	$(LINK.cc) $^ $(OUTPUT_OPTION) $(LAPACK)

$(local_dir)/PowerTest: $(local_dir)/PowerTest.o $(bank_objs) \
	$(random_objs) $(particle_objs) $(mesh_objs) $(hist_objs)
	$(LINK.cc) $^ $(OUTPUT_OPTION) $(LAPACK)

