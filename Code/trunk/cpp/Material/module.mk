# $Author: jlconlin $
# $Id: module.mk 416 2008-11-13 15:13:18Z jlconlin $
# $Revision: 416 $
# $Date: 2008-11-13 08:13:18 -0700 (Thu, 13 Nov 2008) $
 
local_dir := Material
#material_src := $(addprefix $(local_dir)/,Material.cpp)
#local_objs := $(subst .cpp,.o,$(local_src))

#material_objs := $(addprefix $(local_dir)/,Material.o)

modules += $(local_dir)
#sources += $(material_src)
include_dirs += $(local_dir)
#objects += $(material_objs)

#$(material_objs): $(material_src)


