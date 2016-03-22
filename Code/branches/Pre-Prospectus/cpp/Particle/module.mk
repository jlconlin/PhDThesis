# $Author: jlconlin $
# $Id: module.mk 199 2007-11-29 15:15:55Z jlconlin $
# $Revision: 199 $
# $Date: 2007-11-29 08:15:55 -0700 (Thu, 29 Nov 2007) $
 
local_dir := Particle
local_src := $(wildcard $(local_dir)/*.cpp)
local_objs := $(subst .cpp,.o,$(local_src))

modules += $(local_dir)
sources += $(local_src)
include_dirs += $(local_dir)
objects += $(local_objs)

$(local_objs): $(local_src) 
