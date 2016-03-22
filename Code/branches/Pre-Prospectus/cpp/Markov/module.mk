# $Author: jlconlin $
# $Id: module.mk 238 2008-01-05 22:05:09Z jlconlin $
# $Revision: 238 $
# $Date: 2008-01-05 15:05:09 -0700 (Sat, 05 Jan 2008) $
 
local_dir := Markov
local_files := 
local_src := $(addprefix $(local_dir)/,$(local_files))
local_objs := $(subst .cpp,.o,$(local_src))

modules += $(local_dir)
sources += $(local_src)
include_dirs += $(local_dir)
objects += $(local_objs)

$(local_objs): $(local_src) 

