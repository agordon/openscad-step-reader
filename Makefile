# cmake .
# make VERBOSE=1

CPPFLAGS=-I/usr/include/opencascade
CXXFLAGS=-std=c++11 -g -O0

LDFLAGS=-lTKSTL -lTKXDESTEP -lTKBinXCAF -lTKXmlXCAF -lTKXDEIGES -lTKXCAF \
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
