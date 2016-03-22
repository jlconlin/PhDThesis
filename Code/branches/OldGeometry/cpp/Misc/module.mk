# $Author: jlconlin $
# $Id: module.mk 341 2008-08-02 14:45:32Z jlconlin $
# $Revision: 341 $
# $Date: 2008-08-02 08:45:32 -0600 (Sat, 02 Aug 2008) $
 
local_dir := Misc

gnuFile_files := gnuFile.cpp 
gnuFile_src := $(addprefix $(local_dir)/,$(gnuFile_files))
gnuFile_objs := $(subst .cpp,.o,$(gnuFile_src))

local_tests := gnuFileTest LimitsTest
local_pgm := $(addprefix $(local_dir)/,$(local_tests))
local_test_src := $(addsuffix .cpp,$(local_pgm))
local_test_objs := $(addsuffix .o,$(local_pgm))

include_dirs += $(local_dir)
programs += $(local_pgm)
modules += $(local_dir)
sources += $(gnuFile_src) $(local_test_src)
objects += $(local_objs) $(gnuFile_objs) $(local_test_objs)

$(local_dir)/gnuFileTest: $(local_dir)/gnuFileTest.o $(gnuFile_objs)
	$(LINK.cc) $^ $(OUTPUT_OPTION) 

$(local_dir)/LimitsTest: $(local_dir)/LimitsTest.o 
	$(LINK.cc) $^ $(OUTPUT_OPTION) 

