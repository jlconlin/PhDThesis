# $Author: jlconlin $
# $Id: module.mk 248 2008-01-18 14:18:25Z jlconlin $
# $Revision: 248 $
# $Date: 2008-01-18 07:18:25 -0700 (Fri, 18 Jan 2008) $
 
local_dir := FissionSource
local_files := FissionSource.cpp BankSource.cpp HistSource.cpp
local_src := $(addprefix $(local_dir)/,$(local_files))

bank_objs := $(addprefix $(local_dir)/,BankSource.o FissionSource.o) 
hist_objs := $(addprefix $(local_dir)/,HistSource.o FissionSource.o) 

local_objs := $(bank_objs) $(hist_objs)

modules += $(local_dir)
sources += $(local_src)
include_dirs += $(local_dir)
objects += $(bank_objs) $(hist_objs)

$(local_objs): $(local_src) 
