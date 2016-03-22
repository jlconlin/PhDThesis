# $Author: jlconlin $
# $Id: module.mk 237 2008-01-05 15:59:16Z jlconlin $
# $Revision: 237 $
# $Date: 2008-01-05 08:59:16 -0700 (Sat, 05 Jan 2008) $
 
local_dir := FissionSource
local_files := FissionSource.cpp BankSource.cpp HistSource.cpp
local_src := $(addprefix $(local_dir)/,$(local_files))
local_objs := $(subst .cpp,.o,$(local_src))

modules += $(local_dir)
sources += $(local_src)
include_dirs += $(local_dir)
objects += $(local_objs)

$(local_objs): $(local_src) 
