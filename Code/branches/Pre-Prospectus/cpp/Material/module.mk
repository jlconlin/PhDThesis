# $Author: jlconlin $
# $Id: module.mk 187 2007-11-19 17:26:14Z jlconlin $
# $Revision: 187 $
# $Date: 2007-11-19 10:26:14 -0700 (Mon, 19 Nov 2007) $
 
local_dir := Material
local_src := $(addprefix $(local_dir)/,Material.cpp)
local_objs := $(subst .cpp,.o,$(local_src))

modules += $(local_dir)
sources += $(local_src)
include_dirs += $(local_dir)
objects += $(local_objs)

$(local_objs): $(local_src)


