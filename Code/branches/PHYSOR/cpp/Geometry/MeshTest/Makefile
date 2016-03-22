include ../../common.mk

#-------------------- Code and Object Files ----------------------#
CFILES = \
	$(SRCDIR)/Geometry/OneDMesh/OneDCartMesh.cc \
	$(SRCDIR)/Geometry/OneDMesh/OneDNode.cc \
	$(SRCDIR)/Geometry/OneDMesh/OneDZone.cc \
	./MeshTest.cc 
	#$(srcdir)$/OneDMesh/ZoneIterator.cc \
	
OFILES := ${subst .cc,.o,${CFILES}}

.PHONY: TARGET
TARGET : $(OFILES) $(MAKEFILE)

# Rule to make *.o from *.cc
%.o : %.cc
	@echo Compiling $<  
	$(CC) $(CFLAGS) $(INCLUDES) $< -o $@
	@echo

# Delete all object files, core files, exec files, etc...
clean:
	rm -rf $(OFILES) $(TARGET)

depend:
	makedepend $(INCLUDES) $(CFILES)
	
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
./MeshTest.o: ../OneDMesh/OneDCartMesh.hh ../OneDMesh/OneDZone.hh
./MeshTest.o: ../OneDMesh/OneDNode.hh ../Exception/MeshExcept.hh
./MeshTest.o: ../Exception/ExceptionBase.hh
./MeshTest.o: ../OneDMesh/Iterators/MeshTraits.hh
./MeshTest.o: ../OneDMesh/Iterators/GeomIterator.hh
./MeshTest.o: ../Exception/IteratorExcept.hh
./MeshTest.o: ../OneDMesh/Iterators/SweepIter.hh ../Misc/ReverseIterator.hh
