# $Author: jlconlin $
# $Id: module.mk 248 2008-01-18 14:18:25Z jlconlin $
# $Revision: 248 $
# $Date: 2008-01-18 07:18:25 -0700 (Fri, 18 Jan 2008) $
 
local_dir := RandomLib
random_src := $(addprefix $(local_dir)/,Random.cpp)
random_objs := $(subst .cpp,.o,$(random_src))

modules += $(local_dir)
sources += $(random_src)
include_dirs += $(local_dir)
objects += $(random_objs)

$(random_objs): $(random_src) 

