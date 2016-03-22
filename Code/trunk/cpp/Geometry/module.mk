# $Author: jlconlin $
# $Id: module.mk 418 2008-11-17 18:16:28Z jlconlin $
# $Revision: 418 $
# $Date: 2008-11-17 11:16:28 -0700 (Mon, 17 Nov 2008) $
 
local_dir := Geometry
sub_dir := CartesianMesh/OneD TestCodes #CartesianMesh/ThreeD 

include_dirs += $(local_dir)/Field 
include_dirs += $(local_dir)/AngleQuad $(local_dir)/AngleQuad/OneD
include_dirs += $(local_dir)/Exception # $(local_dir)/AngleQuad/ThreeD 
include_dirs += $(local_dir)/Misc $(local_dir)/CartesianMesh

# Get the module file from each subdirectory
include $(addsuffix /module.mk,$(addprefix $(local_dir)/,$(sub_dir)))

