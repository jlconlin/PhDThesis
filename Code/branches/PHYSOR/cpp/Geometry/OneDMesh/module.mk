# $Author: jlconlin $
# $Id: module.mk 248 2008-01-18 14:18:25Z jlconlin $
# $Revision: 248 $
# $Date: 2008-01-18 07:18:25 -0700 (Fri, 18 Jan 2008) $
 
parent_dir := Geometry
local_dir := OneDMesh

# Add all *.hh and *.cc files to local src
#$(warning *.cc wildcard $(wildcard $(parent_dir)/$(local_dir)/*.cc))
#$(warning *.cc wildcard $(parent_dir)/$(local_dir)/)
local_src := $(wildcard $(parent_dir)/$(local_dir)/*.cc)
mesh_objs := $(subst .cc,.o,$(local_src))

sources += $(local_src) 
include_dirs += $(parent_dir)/$(local_dir)
objects += $(mesh_objs)

$(mesh_objs): $(local_src)

