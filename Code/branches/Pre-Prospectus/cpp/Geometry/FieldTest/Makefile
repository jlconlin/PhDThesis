include ../../common.mk

#-------------------- Code and Object Files ----------------------#
CFILES = \
	$(ROOTDIR)/Geometry/OneDMesh/OneDCartMesh.cc \
	$(ROOTDIR)/Geometry/OneDMesh/OneDNode.cc \
	$(ROOTDIR)/Geometry/OneDMesh/OneDZone.cc \
	./FieldTest.cc 
	#$(srcdir)$/OneDMesh/ZoneIterator.cc \
	
OFILES := ${subst .cc,.o,${CFILES}}

.PHONY: TARGET
TARGET : $(OFILES) $(MAKEFILE)

.PHONY: test
test : $(OFILES) $(MAKEFILE)
	$(CC) $(INCLUDES) FieldTest.cc ../OneDMesh/OneDCartMesh.o \
	../OneDMesh/OneDZone.o ../OneDMesh/OneDNode.o -o FieldTest
	./FieldTest

# Rule to make *.o from *.cc
%.o : %.cc
	@echo Compiling $<  
	$(CC) $(CFLAGS) $(INCLUDES) $< -o $@
	@echo

# Delete all object files, core files, exec files, etc...
clean:
	rm -rf $(OFILES) $(TARGET)

depend:
	makedepend $(INCLUDES) $(MDINCLUDES) $(CFILES) 
	
# DO NOT DELETE THIS LINE -- make depend depends on it.

../OneDMesh/OneDCartMesh.o: ../OneDMesh/OneDCartMesh.hh
../OneDMesh/OneDCartMesh.o: ../OneDMesh/OneDZone.hh ../OneDMesh/OneDNode.hh
../OneDMesh/OneDCartMesh.o: ../Exception/MeshExcept.hh
../OneDMesh/OneDCartMesh.o: ../Exception/ExceptionBase.hh
../OneDMesh/OneDCartMesh.o: ../OneDMesh/Iterators/MeshTraits.hh
../OneDMesh/OneDCartMesh.o: ../OneDMesh/Iterators/GeomIterator.hh
../OneDMesh/OneDCartMesh.o: ../Exception/IteratorExcept.hh
../OneDMesh/OneDCartMesh.o: ../OneDMesh/Iterators/SweepIter.hh
../OneDMesh/OneDCartMesh.o: ../Misc/ReverseIterator.hh ../Misc/Assert.hh
../OneDMesh/OneDNode.o: ../OneDMesh/OneDNode.hh ../OneDMesh/OneDZone.hh
../OneDMesh/OneDNode.o: ../Exception/MeshExcept.hh
../OneDMesh/OneDNode.o: ../Exception/ExceptionBase.hh ../Misc/Assert.hh
../OneDMesh/OneDZone.o: ../OneDMesh/OneDZone.hh ../OneDMesh/OneDNode.hh
../OneDMesh/OneDZone.o: ../Exception/MeshExcept.hh
../OneDMesh/OneDZone.o: ../Exception/ExceptionBase.hh ../Misc/Assert.hh
./FieldTest.o: ../OneDMesh/OneDCartMesh.hh ../OneDMesh/OneDZone.hh
./FieldTest.o: ../OneDMesh/OneDNode.hh ../Exception/MeshExcept.hh
./FieldTest.o: ../Exception/ExceptionBase.hh
./FieldTest.o: ../OneDMesh/Iterators/MeshTraits.hh
./FieldTest.o: ../OneDMesh/Iterators/GeomIterator.hh
./FieldTest.o: ../Exception/IteratorExcept.hh
./FieldTest.o: ../OneDMesh/Iterators/SweepIter.hh ../Misc/ReverseIterator.hh
./FieldTest.o: ../Field/Field.hh ../Misc/Assert.hh ../Misc/Iterator.hh
