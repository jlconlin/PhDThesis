# $Author: jlconlin $
# $Id: module.mk 248 2008-01-18 14:18:25Z jlconlin $
# $Revision: 248 $
# $Date: 2008-01-18 07:18:25 -0700 (Fri, 18 Jan 2008) $
 
local_dir := Material
material_src := $(addprefix $(local_dir)/,Material.cpp)
#local_objs := $(subst .cpp,.o,$(local_src))

material_objs := $(addprefix $(local_dir)/,Material.o)

modules += $(local_dir)
sources += $(material_src)
include_dirs += $(local_dir)
objects += $(material_objs)

$(material_objs): $(material_src)


