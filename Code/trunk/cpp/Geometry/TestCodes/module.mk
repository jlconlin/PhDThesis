# $Author: jlconlin $
# $Id: module.mk 423 2008-12-01 20:34:05Z jlconlin $
# $Revision: 423 $
# $Date: 2008-12-01 13:34:05 -0700 (Mon, 01 Dec 2008) $
 
local_dir := Geometry/TestCodes
sub_dir := FieldTest MeshTest #MeshTest3D

# Get the module file from each subdirectory
include $(addsuffix /module.mk,$(addprefix $(local_dir)/,$(sub_dir)))

