# $Author: jlconlin $
# $Id: module.mk 211 2007-12-05 18:06:35Z jlconlin $
# $Revision: 211 $
# $Date: 2007-12-05 11:06:35 -0700 (Wed, 05 Dec 2007) $
 
local_dir := Misc
local_src := $(wildcard $(local_dir)/*.hh)

#sources += $(local_src)
include_dirs += $(local_dir)


