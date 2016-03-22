# $Author: jlconlin $
# $Id: module.mk 285 2008-04-09 16:55:42Z jlconlin $
# $Revision: 285 $
# $Date: 2008-04-09 10:55:42 -0600 (Wed, 09 Apr 2008) $
 
local_dir := Markov
local_files := ArnoldiMC.cpp
local_src := $(addprefix $(local_dir)/,$(local_files))

arnoldiMC_src := $(addprefix $(local_dir)/,ArnoldiMC.cpp)
arnoldiMC_objs := $(subst .cpp,.o,$(arnoldiMC_src))
arnoldiMC_objs += $(arnoldi_objs)

local_tests := ArnoldiMCTest
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

$(local_dir)/ArnoldiMCTest: $(arnoldi_objs) $(arnoldiMC_objs) \
	$(random_objs) $(particle_objs) $(mesh_objs) $(hist_objs) $(material_objs) \
	$(local_dir)/ArnoldiMCTest.o LinearAlgebra/Arnoldi.o
	$(LINK.cc) $^ $(OUTPUT_OPTION) -L/usr/local/lib -lteuchos -framework vecLib

