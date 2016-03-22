# $Author: jlconlin $
# $Id: module.mk 342 2008-08-04 19:55:07Z jlconlin $
# $Revision: 342 $
# $Date: 2008-08-04 13:55:07 -0600 (Mon, 04 Aug 2008) $

local_dir := test
tests := uBlas DateTime
local_pgm := $(addprefix $(local_dir)/test_,$(tests))
local_src := $(addsuffix .cpp,$(local_pgm))
local_objs := $(subst .cpp,.o,$(local_src))

programs += $(local_pgm) 
sources += $(local_src)
program_objects += $(local_objs)

$(local_dir)/test_DateTime: $(local_dir)/test_DateTime.o
	$(LINK.cc) $^ $(OUTPUT_OPTION) $(LAPACK)

$(local_dir)/test_uBlas: $(local_dir)/test_uBlas.o
	$(LINK.cc) $^ $(OUTPUT_OPTION) $(LAPACK)
