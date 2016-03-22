# $Author: jlconlin $
# $Id: module.mk 248 2008-01-18 14:18:25Z jlconlin $
# $Revision: 248 $
# $Date: 2008-01-18 07:18:25 -0700 (Fri, 18 Jan 2008) $
 
local_dir := Particle
local_src := $(wildcard $(local_dir)/*.cpp)
#local_objs := $(subst .cpp,.o,$(local_src))

particle_objs := $(addprefix $(local_dir)/,Particle.o) 

modules += $(local_dir)
sources += $(local_src)
include_dirs += $(local_dir)
objects += $(local_objs)

$(particle_objs): $(local_src) 
