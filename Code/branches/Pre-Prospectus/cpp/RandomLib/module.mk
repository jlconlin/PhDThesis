# $Author: jlconlin $
# $Id: module.mk 188 2007-11-19 18:14:54Z jlconlin $
# $Revision: 188 $
# $Date: 2007-11-19 11:14:54 -0700 (Mon, 19 Nov 2007) $
 
local_dir := RandomLib
local_src := $(addprefix $(local_dir)/,Random.cpp)
local_objs := $(subst .cpp,.o,$(local_src))

modules += $(local_dir)
sources += $(local_src)
include_dirs += $(local_dir)
objects += $(local_objs)

$(local_objs): $(local_src) 

