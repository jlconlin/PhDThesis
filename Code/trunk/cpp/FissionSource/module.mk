# $Author: jlconlin $
# $Id: module.mk 545 2009-05-14 21:08:28Z jlconlin $
# $Revision: 545 $
# $Date: 2009-05-14 15:08:28 -0600 (Thu, 14 May 2009) $
 
local_dir := FissionSource
local_files := FissionSource.cpp BankSource.cpp HistSource.cpp \
	LinearSpaceSource.cpp
local_src := $(addprefix $(local_dir)/,$(local_files))

fs_objs := $(addprefix $(local_dir)/,FissionSource.o) 
bank_objs := $(addprefix $(local_dir)/,BankSource.o FissionSource.o) 
hist_objs := $(addprefix $(local_dir)/,HistSource.o FissionSource.o) 
linspace_objs := $(addprefix $(local_dir)/,FissionSource.o LinearSpaceSource.o) 

local_tests := HistSourceTest LinearSpaceSourceTest
local_pgm := $(addprefix $(local_dir)/,$(local_tests))
local_test_src := $(addsuffix .cpp,$(local_pgm))
local_test_objs := $(addsuffix .o,$(local_pgm))

local_objs := $(bank_objs) $(hist_objs) $(linspace_objs)

modules += $(local_dir)
sources += $(local_src)
include_dirs += $(local_dir)
objects += $(local_objs)
programs += $(local_pgm) $(local_test_objs)

$(local_dir)/HistSourceTest: $(local_dir)/HistSourceTest.o $(hist_objs) \
	$(particle_objs) $(mesh_objs) $(gnuFile_objs) $(random_objs)
	$(LINK.cc) $^ $(OUTPUT_OPTION) $(LAPACK)

$(local_dir)/LinearSpaceSourceTest: $(local_dir)/LinearSpaceSourceTest.o \
	$(particle_objs) $(mesh_objs) $(gnuFile_objs) $(random_objs) \
	$(linspace_objs)
	$(LINK.cc) $^ $(OUTPUT_OPTION) $(LAPACK)

$(local_objs): $(local_src) 
