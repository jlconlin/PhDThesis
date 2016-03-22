# $Author: jlconlin $
# $Id: module.mk 266 2008-03-05 16:44:52Z jlconlin $
# $Revision: 266 $
# $Date: 2008-03-05 09:44:52 -0700 (Wed, 05 Mar 2008) $

local_dir := Gnuplot
local_files := gnuplot_i.cc

local_src := $(addprefix $(local_dir)/,$(local_files))

gnuplot_objs := $(addprefix $(local_dir)/,gnuplot_i.o)

local_objs := $(gnuplot_objs)

modules += $(local_dir)
sources += $(local_src)
include_dirs += $(local_dir)
objects += $(bank_objs) $(hist_objs)

$(local_objs): $(local_src) 
