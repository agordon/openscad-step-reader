## This make file is hard-coded to work on Debian 10 with the following
## packages:
##  apt-get install libocct-data-exchange-dev libocct-draw-dev libocct-foundation-dev \
##                libocct-modeling-algorithms-dev libocct-modeling-data-dev \
##                libocct-ocaf-dev libocct-visualization-dev \
##                libtbb-dev
##
## Other systems will likely need adjustments.
##
## An easy(?) way to see how to build with OpenCASCADE on your system
## is to follow the installation examples of:
##      https://github.com/miho/OCC-CSG/
##
## After building it, run:
##     cmake .
##     make VERBOSE=1
## And you'll see the exact G++ command-line that was used.
##
##

CPPFLAGS=-I/usr/include/opencascade
CXXFLAGS=-std=c++11 -g -O0

LDLIBS=-lTKSTL -lTKXDESTEP -lTKBinXCAF -lTKXmlXCAF -lTKXDEIGES -lTKXCAF \
 -lTKIGES -lTKSTEP -lTKSTEP209 -lTKSTEPAttr -lTKSTEPBase -lTKXSBase \
 -lTKStd -lTKStdL -lTKXml -lTKBin -lTKXmlL -lTKBinL -lTKCAF -lTKXCAF \
 -lTKLCAF -lTKCDF -lTKMeshVS -lTKOpenGl -lTKV3d -lTKService \
 -lTKXMesh -lTKMesh -lTKOffset -lTKFeat -lTKFillet -lTKHLR -lTKBool \
 -lTKBO -lTKShHealing -lTKPrim -lTKTopAlgo -lTKGeomAlgo -lTKBRep \
 -lTKGeomBase -lTKG3d -lTKG2d -lTKIGES -lTKSTEP -lTKSTEP209 \
 -lTKSTEPAttr -lTKSTEPBase -lTKXSBase -lTKStd -lTKStdL -lTKXml \
 -lTKBin -lTKXmlL -lTKBinL -lTKCAF -lTKLCAF -lTKCDF -lTKMeshVS \
 -lTKOpenGl -lTKV3d -lTKService -lTKXMesh -lTKMesh -lTKOffset \
 -lTKFeat -lTKFillet -lTKHLR -lTKBool -lTKBO -lTKShHealing \
 -lTKPrim -lTKTopAlgo -lTKGeomAlgo -lTKBRep -lTKGeomBase \
 -lTKG3d -lTKG2d \
 \
 /usr/lib/x86_64-linux-gnu/libTKMath.so.7.3.0 \
 /usr/lib/x86_64-linux-gnu/libTKernel.so.7.3.0 \
 \
 -lfreetype -lpthread -lrt -lstdc++ -ldl -lm\


all: openscad-step-reader

openscad-step-reader: openscad-step-reader.o \
		      tessellation.o \
		      openscad-triangle-writer.o \
		      explore-shape.o

openscad-step-reader.o: openscad-step-reader.cpp triangle.h

tessellation.o: tessellation.cpp tessellation.h triangle.h

openscad-triangle-writer.o: openscad-triangle-writer.cpp openscad-triangle-writer.h triangle.h

explore-shape.o: explore-shape.cpp explore-shape.h


.PHONY: clean
clean:
	rm -f explore-shape.o openscad-step-reader.o openscad-step-reader tessellation.o openscad-triangle-writer.o
