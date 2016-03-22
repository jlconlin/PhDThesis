# $Author: jlconlin $
# $Id: module.mk 285 2008-04-09 16:55:42Z jlconlin $
# $Revision: 285 $
# $Date: 2008-04-09 10:55:42 -0600 (Wed, 09 Apr 2008) $
 
local_dir := test
#tests := Particle Material Field BankSource HistSource forLeaks Power
#tests := Eigenpair
tests :=  BankSource HistSource Power Gnuplot HistSource # pyFile
local_pgm := $(addprefix $(local_dir)/test_,$(tests))
#local_pgm := $(local_dir)/test_Particle $(local_dir)/test_Material
local_src := $(addsuffix .cpp,$(local_pgm))
local_objs := $(subst .cpp,.o,$(local_src))

programs += $(local_pgm)  $(local_dir)/test_Arnoldi \
			$(local_dir)/test_Teuchos $(local_dir)/test_Anasazi
sources += $(local_src) $(local_dir)/test_Arnoldi.cpp \
			$(local_dir)/test_Teuchos.cpp $(local_dir)/test_Anasazi.cpp
programs_objects += $(local_objs) \
#				$(local_dir)/test_Teuchos.o $(local_dir)/test_Anasazi.o


#test_Gnuplot
$(local_dir)/test_Gnuplot: $(gnuplot_objs) $(local_dir)/test_Gnuplot.o
	$(LINK.cc) $^ $(OUTPUT_OPTION) 

#test_Arnoldi
$(local_dir)/test_Arnoldi: $(arnoldi_objs) $(arnoldiMatrix_objs) \
	$(random_objs) $(material_objs) $(particle_objs) $(hist_objs) $(mesh_objs) \
   	$(bank_objs) $(arnoldiMC_objs) $(gnuplot_objs) $(local_dir)/test_Arnoldi.o
	$(LINK.cc) $^ $(OUTPUT_OPTION) -L/usr/local/lib -lteuchos -framework vecLib

#test_Anasazi
$(local_dir)/test_Anasazi: $(local_dir)/test_Anasazi.o 
	$(LINK.cc) $^  -framework vecLib \
	-L/usr/local/lib -lteuchos -lanasazi -lepetra  $(OUTPUT_OPTION)

#test_Teuchos
$(local_dir)/test_Teuchos: $(local_dir)/test_Teuchos.o 
	$(LINK.cc) $^ -L/usr/local/lib -lteuchos -framework vecLib $(OUTPUT_OPTION)

#test_BankSource
$(local_dir)/test_BankSource: $(bank_objs) $(particle_objs) $(random_objs) \
	$(mesh_objs) $(material_objs) $(local_dir)/test_BankSource.o
	$(LINK.cc) $^  $(OUTPUT_OPTION)

#test_Eigenpair
$(local_dir)/test_Eigenpair: $(eigenpair_objs) $(local_dir)/test_Eigenpair.o
	$(LINK.cc) $^  $(OUTPUT_OPTION)

#test_HistSource
$(local_dir)/test_HistSource: $(hist_objs) $(particle_objs) $(random_objs) \
	$(mesh_objs) $(material_objs) $(gnuplot_objs) $(local_dir)/test_HistSource.o
	$(LINK.cc) $^  $(OUTPUT_OPTION)

#test_Power
$(local_dir)/test_Power: $(hist_objs) $(particle_objs) $(random_objs) \
	$(bank_objs) $(mesh_objs) $(material_objs) $(local_dir)/test_Power.o
	$(LINK.cc) $^  $(OUTPUT_OPTION)

#test_pyFile
(local_dir)/test_pyFile: $(local_dir)/test_pyFile.o
	$(LINK.cc) $^ $(OUTPUT_OPTION) 

#$(local_pgm): $(programs_objects) $(objects) 
	#$(LINK.cc) $(objects) $(addsuffix .o,$@) $(OUTPUT_OPTION) 
