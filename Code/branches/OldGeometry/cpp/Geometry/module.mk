# $Author: jlconlin $
# $Id: module.mk 211 2007-12-05 18:06:35Z jlconlin $
# $Revision: 211 $
# $Date: 2007-12-05 11:06:35 -0700 (Wed, 05 Dec 2007) $
 
local_dir := Geometry
sub_dir := Field OneDMesh

# Get the module file from each subdirectory
include $(addsuffix /module.mk,$(addprefix $(local_dir)/,$(sub_dir)))
