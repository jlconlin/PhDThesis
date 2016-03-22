# $Author: jlconlin $
# $Id: module.mk 423 2008-12-01 20:34:05Z jlconlin $
# $Revision: 423 $
# $Date: 2008-12-01 13:34:05 -0700 (Mon, 01 Dec 2008) $

local_dir := Geometry/TestCodes/IteratorTest
local_tests := IteratorTest
local_files := $(addsuffix .cc,$(local_tests) )
local_src := $(addprefix $(local_dir)/,$(local_files))
local_objs := $(subst .cc,.o, $(local_src))

local_pgm := $(addprefix $(local_dir)/,$(local_tests))

programs += $(local_pgm)
objects += $(local_objs)
sources += $(local_src)

$(local_pgm): $(local_objs) $(mesh_objs)
	$(LINK.cc) -I $(local_dir) $^ $(OUTPUT_OPTION) $(LAPACK)
