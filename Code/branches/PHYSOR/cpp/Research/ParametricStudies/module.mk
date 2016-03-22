# $Author: jlconlin $
# $Id: module.mk 297 2008-05-09 13:21:30Z jlconlin $
# $Revision: 297 $
# $Date: 2008-05-09 07:21:30 -0600 (Fri, 09 May 2008) $

parent_dir := Research
local_dir := $(parent_dir)/ParametricStudies
studies := IterationHistories IterationBins BinsBias
local_pgm := $(addprefix $(local_dir)/,$(studies))
local_src := $(addsuffix .cpp,$(local_pgm))
local_objs := $(addsuffix .o,$(local_pgm))

sources += $(local_src)
programs += $(local_pgm)
programs_objs += $(local_objs)

$(local_dir)/BinsBias:  $(local_dir)/BinsBias.o $(arnoldi_objs) \
	$(random_objs) $(material_objs) $(particle_objs) $(hist_objs) $(mesh_objs) \
   	$(bank_objs) $(arnoldiMC_objs) $(gnuplot_objs) $(gnuFile_objs)
	$(LINK.cc) $^ $(OUTPUT_OPTION) -L/usr/local/lib -lteuchos \
		-lboost_program_options-1_34_1 -framework vecLib

$(local_dir)/IterationBins:  $(local_dir)/IterationBins.o $(arnoldi_objs) \
	$(random_objs) $(material_objs) $(particle_objs) $(hist_objs) $(mesh_objs) \
   	$(bank_objs) $(arnoldiMC_objs) $(gnuplot_objs) $(gnuFile_objs)
	$(LINK.cc) $^ $(OUTPUT_OPTION) -L/usr/local/lib -lteuchos \
		-lboost_program_options-1_34_1 -framework vecLib

$(local_dir)/IterationHistories:  $(local_dir)/IterationHistories.o $(arnoldi_objs) \
	$(random_objs) $(material_objs) $(particle_objs) $(hist_objs) $(mesh_objs) \
   	$(bank_objs) $(arnoldiMC_objs) $(gnuplot_objs) $(gnuFile_objs)
	$(LINK.cc) $^ $(OUTPUT_OPTION) -L/usr/local/lib -lteuchos \
		-lboost_program_options-1_34_1 -framework vecLib
