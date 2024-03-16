/*
 * Copyright 2019 Assaf Gordon <assafgordon@gmail.com>
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU Lesser General Public License for more details.
 */
#include <ostream>
#include <iostream>
#include <vector>

#include <gp_Pnt.hxx>

#include "triangle.h"

using namespace std;


/* Write the faces/triangles as an ASCII stl file
   (with invalud 'normals' value - but these are ignored anyhow in OpenSCAD */
void write_triangles_ascii_stl(const Face_vector& faces)
{
	cout << "solid" << std::endl;
	for (auto &f : faces)
		f.write_ascii_stl(cout);
	cout << "endsolid" << std::endl;
}

/* Write the faces/triangles as two vectors (one "POINTS", one "FACES")
   that will be used with a single call to "polyhedron"). */
void write_triangle_scad(const Face_vector& faces)
{
	Face all;

	// Merge all faces (i.e. triangles from all faces)
	// into one "face" (just a container, no special meaning of a "face").
	for (auto &f : faces)
		all.add_face(f);

	// Write vector of points and faces
	cout << "points = " ;
	all.write_points_vector(cout);
	cout << "faces = ";
	all.write_face_vector(cout);

	// Call Polyhedron
	cout << "module solid_object() {" << std::endl;
	cout << "  polyhedron (points,faces);"<< std::endl;
	cout << "}" << std::endl;
	cout << std::endl;
	cout << "solid_object();" << std::endl;
}




#define NUM_COLORS 12
const char* colors[NUM_COLORS] = {
	"black",
	"Violet",
	"red",
	"blue",
	"LawnGreen",
	"Orange",
	"DeepPink",
	"Gold",
	"Cyan",
	"Olive",
	"Gray",
	"SpringGreen"
};


/* Write every faces (i.e. all trianges of each face) into a separate points/faces
   vector pairs.

   Include $preview code to show each face in a different color
   (sadly, resulting in an invalid 3D object which can't be exported to STL,
   because each face is a 2D object with depth of ZERO).

   In non-preview mode,
   Include code to merge all the vectors and make a single "Polyhedron" call. */
void write_faces_scad (const Face_vector& faces)
{
	int i = 1;
	for (auto &f : faces) {
		cout << "face_" << i << "_points = " ;
		f.write_points_vector(cout);
		cout << "face_" << i << "_faces = " ;
		f.write_face_vector(cout);
		cout << endl ;
		++i;
	}

	/* crazy colors version, draw each face by itself */
	cout << "module crazy_colors() {" << std::endl;
	for (i=1;i<=faces.size();++i) {
		const char* color = colors[i%NUM_COLORS] ;
		cout << "color(\"" << color << "\")" << std::endl;
		cout << "polyhedron(face_" << i <<"_points, face_" << i << "_faces);" << endl ;
	}
	cout << "}" << std::endl;

	cout << "function add_offset(vec,ofs) = [for (x=vec) x + [ofs,ofs,ofs]];" << std::endl;
	cout << "module solid_object() {" << std::endl;
	cout << "  tmp1_points = face_1_points;" << std::endl;
	cout << "  tmp1_faces =  face_1_faces;" << std::endl;
	cout << std::endl;
	for (i=2;i<=faces.size();++i) {
		cout << "  tmp"<<i<<"_points = concat(tmp"<<(i-1)<<"_points, face_"<<i<<"_points);" << std::endl;
		cout << "  tmp"<<i<<"_faces =  concat(tmp"<<(i-1)<<"_faces,add_offset(face_"<<i<<"_faces,len(tmp"<<(i-1)<<"_points)));" << std::endl;
		cout << std::endl;
	}
	cout << "  polyhedron (tmp"<<(faces.size())<<"_points, tmp"<<(faces.size())<<"_faces);"<< std::endl;
	cout << "}" << std::endl;
	cout << std::endl;
	cout << std::endl;

	cout << "if ($preview) {;" << std::endl;
	cout << "  crazy_colors();" << std::endl;
	cout << "} else {" << std::endl;
	cout << "  solid_object();" << std::endl;
	cout << "}" << std::endl;
}
