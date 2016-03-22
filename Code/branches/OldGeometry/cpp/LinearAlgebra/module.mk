# $Author: jlconlin $
# $Id: module.mk 318 2008-06-25 20:20:56Z jlconlin $
# $Revision: 318 $
# $Date: 2008-06-25 14:20:56 -0600 (Wed, 25 Jun 2008) $
 
local_dir := LinearAlgebra
local_files := Arnoldi.cpp ArnoldiMatrix.cpp Eigenpair.cpp
arnoldi_src := $(addprefix $(local_dir)/,$(local_files))

arnoldi_objs := $(addprefix $(local_dir)/,Arnoldi.o)
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
	$(random_objs) $(arnoldi_objs) $(arnoldiMatrix_objs) 
	$(LINK.cc) $^ $(OUTPUT_OPTION) $(LAPACK)

$(local_dir)/ArnoldiTest: $(local_dir)/ArnoldiTest.o $(arnoldi_objs)
	$(LINK.cc) $^ $(OUTPUT_OPTION) $(LAPACK)

$(local_dir)/QRAlgoTest: $(local_dir)/QRAlgoTest.o
	$(LINK.cc) $^ $(OUTPUT_OPTION) $(LAPACK)
