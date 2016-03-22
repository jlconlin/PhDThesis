# $Author: jlconlin $
# $Id: module.mk 218 2007-12-13 16:30:57Z jlconlin $
# $Revision: 218 $
# $Date: 2007-12-13 09:30:57 -0700 (Thu, 13 Dec 2007) $
 
parent_dir := Geometry
local_dir := OneDMesh

# Add all *.hh and *.cc files to local src
#$(warning *.cc wildcard $(wildcard $(parent_dir)/$(local_dir)/*.cc))
#$(warning *.cc wildcard $(parent_dir)/$(local_dir)/)
local_src := $(wildcard $(parent_dir)/$(local_dir)/*.cc)
local_objs := $(subst .cc,.o,$(local_src))

sources += $(local_src) 
include_dirs += $(parent_dir)/$(local_dir)
objects += $(local_objs)

