# $Author: jlconlin $
# $Id: module.mk 282 2008-04-04 13:59:33Z jlconlin $
# $Revision: 282 $
# $Date: 2008-04-04 07:59:33 -0600 (Fri, 04 Apr 2008) $
 
local_dir := LinearAlgebra
local_files := Arnoldi.cpp ArnoldiMatrix.cpp Eigenpair.cpp
arnoldi_src := $(addprefix $(local_dir)/,$(local_files))

arnoldi_objs := $(addprefix $(local_dir)/,Arnoldi.o)
arnoldiMatrix_objs := $(addprefix $(local_dir)/,ArnoldiMatrix.o)
eigenpair_objs := $(addprefix $(local_dir)/,Eigenpair.o)

local_tests := ArnoldiTest ArnoldiMatrixTest
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
	$(LINK.cc) $^ $(OUTPUT_OPTION) -L/usr/local/lib -lteuchos -framework vecLib

$(local_dir)/ArnoldiTest: $(local_dir)/ArnoldiTest.o $(arnoldi_objs)
	$(LINK.cc) $^ $(OUTPUT_OPTION) -L/usr/local/lib -lteuchos -framework vecLib

