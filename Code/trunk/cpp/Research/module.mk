# $Author: jlconlin $
# $Id: module.mk 654 2009-06-26 15:03:02Z jlconlin $
# $Revision: 654 $
# $Date: 2009-06-26 09:03:02 -0600 (Fri, 26 Jun 2009) $
 
local_dir := Research

local_pgm := ArnoldivsPower ArnoldiResearch PowerResearch HeteroArnoldi
local_src := $(addprefix $(local_dir)/,$(addsuffix .cpp,$(local_pgm)))

local_objs := $(addprefix $(local_dir)/,$(addsuffix .o,$(local_pgm)))

include_dir += $(local_dir)
programs += $(addprefix $(local_dir)/,$(local_pgm))
modules += $(local_dir)
sources += $(local_src)
objects += $(local_objs)

$(local_dir)/LooselyCoupled: $(local_dir)/LooselyCoupled.o \
	$(random_objs) $(particle_objs) $(mesh_objs) $(hist_objs) $(bank_objs) \
	$(gnuFile_objs) $(linspace_objs)
	$(LINK.cc) $^ $(OUTPUT_OPTION) $(LAPACK)

$(local_dir)/HeteroArnoldi: $(local_dir)/HeteroArnoldi.o \
	$(random_objs) $(particle_objs) $(mesh_objs) $(hist_objs) $(bank_objs) \
	$(gnuFile_objs) $(linspace_objs) $(arg_objs)
	$(warning local objects: $(local_objs))
	$(LINK.cc) $^ $(OUTPUT_OPTION) $(LAPACK)

$(local_dir)/ArnoldiResearch: $(local_dir)/ArnoldiResearch.o \
	$(random_objs) $(particle_objs) $(mesh_objs) $(hist_objs) $(bank_objs) \
	$(gnuFile_objs) $(linspace_objs) $(arg_objs)
	$(warning local objects: $(local_objs))
	$(LINK.cc) $^ $(OUTPUT_OPTION) $(LAPACK)

$(local_dir)/PowerResearch: $(local_dir)/PowerResearch.o \
	$(random_objs) $(particle_objs) $(mesh_objs) $(hist_objs) $(bank_objs) \
	$(gnuFile_objs) $(linspace_objs) $(arg_objs)
	$(LINK.cc) $^ $(OUTPUT_OPTION) $(LAPACK)

$(local_dir)/ArnoldivsPower: $(local_dir)/ArnoldivsPower.o \
	$(random_objs) $(particle_objs) $(mesh_objs) $(hist_objs) $(bank_objs) \
	$(gnuFile_objs) $(linspace_objs)
	$(LINK.cc) $^ $(OUTPUT_OPTION) $(LAPACK)
