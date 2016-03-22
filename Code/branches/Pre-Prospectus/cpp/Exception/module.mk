# $Author: jlconlin $
# $Id: module.mk 219 2007-12-13 21:59:56Z jlconlin $
# $Revision: 219 $
# $Date: 2007-12-13 14:59:56 -0700 (Thu, 13 Dec 2007) $

local_dir := Exception
#local_src := $(wildcard $(local_dir)/*.h) $(wildcard $(local_dir)/*.hh)

#sources += $(local_src)
include_dirs += $(local_dir)
