# target name
TARGET=Mesh
include ../../common.mk

CCMESHFILES=OneDCartMesh.cc OneDZone.cc OneDNode.cc
HHMESHFILES=OneDCartMesh.hh OneDZone.hh OneDNode.hh
OMESHFILES=OneDCartMesh.o OneDZone.o OneDNode.o

#------------------------- Rules -------------------------#


_$(TARGET).so : OneDCartMesh_wrap.o 
	$(CC) $(LDFLAGS) $(OMESHFILES) OneDCartMesh_wrap.o -o _$(TARGET).so $(INCLUDES)
	$(pyc-copy)

OneDCartMesh_wrap.o :  $(CCMESHFILES) $(HHMESHFILES) OneDCartMesh_wrap.cxx 
	$(CC) $(CFLAGS) $(CCMESHFILES) OneDCartMesh_wrap.cxx  $(PYINCLUDES) $(INCLUDES)

OneDCartMesh_wrap.cxx  :  OneDCartMesh.i 
	$(SWIG) $(SWIGFLAGS) $(INCLUDES) OneDCartMesh.i 

.PHONY: clean
clean : ; rm -f *.o *.cxx *.so *.pyc

.PHONY: check
check : ; $(PYTHON) Test$(TARGET).py
