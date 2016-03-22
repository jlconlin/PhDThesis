# $Author: jlconlin $
# $Id: Makefile 183 2007-11-15 21:00:05Z jlconlin $
# $Revision: 183 $
# $Date: 2007-11-15 14:00:05 -0700 (Thu, 15 Nov 2007) $

include ../common.mk

INCLUDES += -I Misc.i/

SHELL = /bin/sh

SOURCEDIRS=OneDMesh Field

.PHONY: $(SOURCEDIRS)

all: $(SOURCEDIRS)

$(SOURCEDIRS):
	@echo "Making everything in" $@
	$(MAKE) --directory=$@;
	@echo "--------------------------------------------------" 
