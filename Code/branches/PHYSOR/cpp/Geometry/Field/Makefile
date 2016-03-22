# target name
TARGET=Field
include ../../common.mk


#------------------------- Rules -------------------------#


_$(TARGET).so : $(TARGET)_wrap.o 
	$(CC) $(LDFLAGS) $(TARGET)_wrap.o $(SRCDIR)/Geometry/OneDMesh/*.o \
	$(SRCDIR)/Material/Material.o -o _$(TARGET).so $(INCLUDES)
	$(pyc-copy)

$(TARGET)_wrap.o :  Field.hh $(TARGET)_wrap.cxx 
	$(CC) $(CFLAGS) $(TARGET)_wrap.cxx  $(PYINCLUDES) $(INCLUDES)

$(TARGET)_wrap.cxx  :  $(TARGET).i 
	$(SWIG) $(SWIGFLAGS) $(INCLUDES) $(TARGET).i 

.PHONY: clean
clean : ; rm -f *.o *.cxx *.so *.pyc

.PHONY: check
check : ; $(PYTHON) Test$(TARGET).py
