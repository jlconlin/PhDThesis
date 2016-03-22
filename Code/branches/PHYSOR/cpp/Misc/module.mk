# $Author: jlconlin $
# $Id: module.mk 297 2008-05-09 13:21:30Z jlconlin $
# $Revision: 297 $
# $Date: 2008-05-09 07:21:30 -0600 (Fri, 09 May 2008) $
 
local_dir := Misc

gnuFile_files := gnuFile.cpp 
gnuFile_src := $(addprefix $(local_dir)/,$(gnuFile_files))
gnuFile_objs := $(subst .cpp,.o,$(gnuFile_src))

local_tests := gnuFileTest
local_pgm := $(addprefix $(local_dir)/,$(local_tests))
local_test_src := $(addsuffix .cpp,$(local_pgm))
local_test_objs := $(addsuffix .o,$(local_pgm))

include_dirs += $(local_dir)
programs += $(local_pgm)
modules += $(local_dir)
sources += $(gnuFile_src) $(local_test_src)
objects += $(local_objs) $(gnuFile_objs) $(local_test_objs)

$(local_pgm): $(local_test_objs) $(gnuFile_objs)
	$(LINK.cc) $^ $(OUTPUT_OPTION) 

