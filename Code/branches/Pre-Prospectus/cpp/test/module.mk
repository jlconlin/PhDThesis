# $Author: jlconlin $
# $Id: module.mk 237 2008-01-05 15:59:16Z jlconlin $
# $Revision: 237 $
# $Date: 2008-01-05 08:59:16 -0700 (Sat, 05 Jan 2008) $
 
local_dir := test
tests := Particle Material Field BankSource HistSource forLeaks Power
local_pgm := $(addprefix $(local_dir)/test_,$(tests))
#local_pgm := $(local_dir)/test_Particle $(local_dir)/test_Material
local_src := $(addsuffix .cpp,$(local_pgm))
local_objs := $(subst .cpp,.o,$(local_src))

programs += $(local_pgm)
#include_dirs += $(local_inc)
sources += $(local_src)
programs_objects += $(local_objs)
#objects += $(local_objs)

$(local_pgm): $(programs_objects) $(objects) 
	$(warning objects = $(objects))
	$(CC) $(CFLAGS) $(CPPFLAGS) $(TARGET_ARCH) $(objects) $(addsuffix .o,$@) -o $@
