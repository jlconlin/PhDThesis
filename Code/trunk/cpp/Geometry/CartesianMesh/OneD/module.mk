# $Author: jlconlin $
# $Id: module.mk 349 2008-08-07 18:32:50Z jlconlin $
# $Revision: 349 $
# $Date: 2008-08-07 12:32:50 -0600 (Thu, 07 Aug 2008) $
 
parent_dir := Geometry/CartesianMesh
local_dir := OneD

local_src := $(wildcard $(parent_dir)/$(local_dir)/*.cc)
mesh_objs := $(subst .cc,.o,$(local_src))

sources += $(local_src) 
include_dirs += $(parent_dir)/$(local_dir)
objects += $(mesh_objs)

$(mesh_objs): $(local_src)

