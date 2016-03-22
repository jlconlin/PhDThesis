# $Author: jlconlin $
# $Id: module.mk 544 2009-05-14 14:56:51Z jlconlin $
# $Revision: 544 $
# $Date: 2009-05-14 08:56:51 -0600 (Thu, 14 May 2009) $
 
local_dir := Benchmark

local_pgm := Benchmark
local_src := $(addprefix $(local_dir)/,$(addsuffix .cpp,$(local_pgm)))

local_objs := $(addsuffix .o,$(local_pgm))

programs += $(addprefix $(local_dir)/,$(local_pgm))
modules += $(local_dir)
sources += $(local_src)
objects += $(local_objs)

$(local_dir)/Benchmark: $(local_dir)/Benchmark.o \
	$(random_objs) $(particle_objs) $(mesh_objs) $(hist_objs) $(bank_objs) \
	$(gnuFile_objs)
	$(LINK.cc) $^ $(OUTPUT_OPTION) $(LAPACK)
