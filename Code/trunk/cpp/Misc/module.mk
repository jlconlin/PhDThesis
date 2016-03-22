# $Author: jlconlin $
# $Id: module.mk 549 2009-05-18 21:04:40Z jlconlin $
# $Revision: 549 $
# $Date: 2009-05-18 15:04:40 -0600 (Mon, 18 May 2009) $
 
local_dir := Misc

arg_files := Arguments.cpp
arg_src := $(addprefix $(local_dir)/,$(arg_files))
arg_objs := $(subst .cpp,.o,$(arg_src))

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
objects += $(local_objs) $(gnuFile_objs) $(arg_objs) $(local_test_objs)

$(local_dir)/gnuFileTest: $(local_dir)/gnuFileTest.o $(gnuFile_objs)
	$(LINK.cc) $^ $(OUTPUT_OPTION) 

$(local_dir)/LimitsTest: $(local_dir)/LimitsTest.o 
	$(LINK.cc) $^ $(OUTPUT_OPTION) 

