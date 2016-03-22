# $Author: jlconlin $
# $Id: module.mk 401 2008-10-17 21:11:31Z jlconlin $
# $Revision: 401 $
# $Date: 2008-10-17 15:11:31 -0600 (Fri, 17 Oct 2008) $
 
local_dir := LinearAlgebra
local_files := ArnoldiMatrix.cpp Eigenpair.cpp
arnoldi_src := $(addprefix $(local_dir)/,$(local_files))

arnoldiMatrix_objs := $(addprefix $(local_dir)/,ArnoldiMatrix.o)
eigenpair_objs := $(addprefix $(local_dir)/,Eigenpair.o)

local_tests := ArnoldiTest ArnoldiMatrixTest QRAlgoTest
local_pgm := $(addprefix $(local_dir)/,$(local_tests))
local_test_src := $(addsuffix .cpp,$(local_pgm))
local_test_objs := $(addsuffix .o,$(local_pgm))

programs += $(local_pgm)
modules += $(local_dir)
sources += $(arnoldi_src)	$(local_test_src)
include_dirs += $(local_dir)
objects += $(arnoldi_objs) $(arnoldiMatrix_objs) $(local_test_objs)

$(arnoldi_objs) $(arnoldiMatrix_objs): $(arnoldi_src) 

$(local_dir)/ArnoldiMatrixTest: $(local_dir)/ArnoldiMatrixTest.o \
	$(random_objs) $(arnoldiMatrix_objs) 
	$(LINK.cc) $^ $(OUTPUT_OPTION) $(LAPACK)

$(local_dir)/ArnoldiTest: $(local_dir)/ArnoldiTest.o 
	$(LINK.cc) $^ $(OUTPUT_OPTION) $(LAPACK)

$(local_dir)/QRAlgoTest: $(local_dir)/QRAlgoTest.o
	$(LINK.cc) $^ $(OUTPUT_OPTION) $(LAPACK)
