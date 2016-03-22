# $Author: jlconlin $
# $Id: module.mk 515 2009-04-10 21:08:43Z jlconlin $
# $Revision: 515 $
# $Date: 2009-04-10 15:08:43 -0600 (Fri, 10 Apr 2009) $

local_dir := test
tests := uBlas DateTime Geometry gnuplot tclap BoostVariant BoostAccumulator \
	CPP miniLS general
local_pgm := $(addprefix $(local_dir)/test_,$(tests))
local_src := $(addsuffix .cpp,$(local_pgm))
local_objs := $(subst .cpp,.o,$(local_src))

programs += $(local_pgm) 
sources += $(local_src)
program_objects += $(local_objs)

$(local_dir)/test_general: $(local_dir)/test_general.o
	$(LINK.cc) $^ $(OUTPUT_OPTION) 

$(local_dir)/test_miniLS: $(local_dir)/test_miniLS.o $(gnuplot_objs) \
	$(mesh_objs) $(random_objs) $(fs_objs)
	$(LINK.cc) $^ $(OUTPUT_OPTION) 

$(local_dir)/test_tclap: $(local_dir)/test_tclap.o 
	$(LINK.cc) $^ $(OUTPUT_OPTION) $(LAPACK)

$(local_dir)/test_gnuplot: $(local_dir)/test_gnuplot.o $(gnuplot_objs)
	$(LINK.cc) $^ $(OUTPUT_OPTION) $(LAPACK)

$(local_dir)/test_DateTime: $(local_dir)/test_DateTime.o
	$(LINK.cc) $^ $(OUTPUT_OPTION) $(LAPACK)

$(local_dir)/test_CPP: $(local_dir)/test_CPP.o 
	$(LINK.cc) $^ $(OUTPUT_OPTION) 

$(local_dir)/test_Geometry: $(local_dir)/test_Geometry.o $(mesh_objs)
	$(LINK.cc) $^ $(OUTPUT_OPTION) $(LAPACK)

$(local_dir)/test_BoostAccumulator: $(local_dir)/test_BoostAccumulator.o
	$(LINK.cc) $^ $(OUTPUT_OPTION) $(LAPACK)

$(local_dir)/test_BoostVariant: $(local_dir)/test_BoostVariant.o
	$(LINK.cc) $^ $(OUTPUT_OPTION) $(LAPACK)

$(local_dir)/test_uBlas: $(local_dir)/test_uBlas.o
	$(LINK.cc) $^ $(OUTPUT_OPTION) $(LAPACK)

