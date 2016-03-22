# $Author: jlconlin $
# $Id: module.mk 295 2008-05-05 17:23:46Z jlconlin $
# $Revision: 295 $
# $Date: 2008-05-05 11:23:46 -0600 (Mon, 05 May 2008) $

local_dir := Research
files := ArnoldiVsPower
local_pgm := $(addprefix $(local_dir)/,$(files))
local_src := $(addsuffix .cpp,$(local_pgm))
local_objs := $(addsuffix .o,$(local_pgm))

sources += $(local_src)
programs += $(local_pgm)
programs_objs += $(local_objs)

$(local_dir)/ArnoldiVsPower: $(arnoldi_objs) $(arnoldiMatrix_objs) \
	$(random_objs) $(material_objs) $(particle_objs) $(hist_objs) $(mesh_objs) \
   	$(bank_objs) $(arnoldiMC_objs) $(gnuplot_objs) $(gnuFile_objs) \
	$(local_dir)/ArnoldiVsPower.o
	$(LINK.cc) $^ $(OUTPUT_OPTION) -L/usr/local/lib -lteuchos \
		-lboost_program_options-1_34_1 -framework vecLib
