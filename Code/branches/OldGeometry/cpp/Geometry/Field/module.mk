# $Author: jlconlin $
# $Id: module.mk 188 2007-11-19 18:14:54Z jlconlin $
# $Revision: 188 $
# $Date: 2007-11-19 11:14:54 -0700 (Mon, 19 Nov 2007) $
 
parent_dir := Geometry
local_dir := Field
local_src := $(wildcard $(local_dir)/*.hh)

sources += $(local_src)
include_dirs += $(addprefix $(parent_dir)/,$(local_dir))

