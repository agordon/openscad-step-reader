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
#include <iostream>
#include <string>
#include <err.h>
#include <getopt.h>

#include <STEPControl_Reader.hxx>
#include <StlAPI_Writer.hxx>
#include <BRepMesh_IncrementalMesh.hxx>

#include "triangle.h"
#include "tessellation.h"
#include "openscad-triangle-writer.h"
#include "explore-shape.h"

static struct option options[] = {
	{"help",      no_argument, 0, 'h' },
	{"version",   no_argument, 0, 'V' },
	{"stl-ascii", no_argument, 0, 'a' },
	{"stl-scad",  no_argument, 0, 's' },
	{"stl-faces", no_argument, 0, 'f' },
	{"stl-occt",  no_argument, 0, 'o' },
	{"stl-lin-tol", required_argument, 0, 'L'},
	{"explore",   no_argument, 0, 'e' },
};

void show_help()
{

	std::cout << "openscad-step-reader\n"
"\n"
"A proof-of-concept program for STEP/OpenSCAD integration\n"
"\n"
"usage: openscad-step-reader [options] INPUT.STEP\n"
"\n"
"Output is written to STDOUT.\n"
"\n"
"options are:\n"
"   -h, --help         this help screen\n"
"   -V, --version      version information\n"
"\n"
"   -o, --stl-occt     convert the input STEP file into ASCII STL file\n"
"                      using OpenCASCADE code. This should be the baseline\n"
"                      when debugging/troubleshooting incorrect outputs.\n"
"\n"
"   -a, --stl-ascii    convert the input STEP file into custom ASCII STL file,\n"
"                      using our code. This is a good test to check mesh\n"
"                      triangulation code. EXCEPT for the 'normal' values\n"
"                      which are not produced, the vertex values should be\n"
"                      equivalent to those with --stl-occt.\n"
"\n"
"   -s, --stl-scad     convert the input STEP file into SCAD code, containing\n"
"                      a single 'polyhedron' call with the STL triangles stored\n"
"                      in SCAD vectors.\n"
"\n"
"   -f, --stl-faces    convert the input STEP file into SCAD code, retaining the\n"
"                      'face' information from the STEP file. Each face will be rendered\n"
"                      in a different color in openscad $preview mode.\n"
"\n"
"   -e, --explore      Work-in-progress code, used for development and exploration\n"
"                      of OpenCASCADE class hierarchy, e.g.\n"
"                      Shell->Face->Surface->Wire->Edge->Vertex.\n"
"                      produces debug messges and no useful output.\n"
"\n"
"Written by Assaf Gordon (assafgordon@gmail.com)\n"
"License: LGPLv2.1 or later\n"
"\n";
	exit(0);
}

void show_version()
{
	std::cout << 42 << std::endl;
	exit(0);
}


int main(int argc, char *argv[])
{
	double stl_lin_tol = 0.5 ; //default linear tolerace.
	int c;

	enum {
		OUT_UNDEFINED,
		OUT_STL_ASCII,
		OUT_STL_SCAD,
		OUT_STL_FACES,
		OUT_STL_OCCT,
		OUT_EXPLORE
	} output = OUT_UNDEFINED;

	while ((c = getopt_long(argc, argv, "hVasfoL:e", options, NULL))!=-1) {
		switch (c)
		{
		case 'h':
			show_help();
			break;

		case 'v':
			show_version();
			break;

		case 'a':
			output = OUT_STL_ASCII;
			break;

		case 's':
			output = OUT_STL_SCAD;
			break;

		case 'f':
			output = OUT_STL_FACES;
			break;

		case 'o':
			output = OUT_STL_OCCT;
			break;

		case 'e':
			output = OUT_EXPLORE;
			break;

		case 'L':
			stl_lin_tol = atof(optarg);
			if (stl_lin_tol<=0)
				errx(1,"invalid tolerance value '%s'",optarg);
			break;

		}
	}

	if (optind >= argc)
		errx(1,"missing input STEP filename. Use --help for usage information");

	if (output == OUT_UNDEFINED)
		errx(1,"missing output format option. Use --help for usage information");

	std::string filename(argv[optind]);

	/* Load the shape from STEP file.
	   See https://github.com/miho/OCC-CSG/blob/master/src/occ-csg.cpp#L311
	   and https://github.com/lvk88/OccTutorial/blob/master/OtherExamples/runners/convertStepToStl.cpp
	 */
	TopoDS_Shape shape;

	STEPControl_Reader Reader;
	enum IFSelect_ReturnStatus s = Reader.ReadFile(filename.c_str());
	if (s != IFSelect_RetDone)
		err(1,"failed to load STEP file '%s'", filename.c_str());
	Reader.TransferRoots();
	shape = Reader.OneShape();

	/* Is this required (for Tessellation and/or StlAPI_Writer?) */
	BRepMesh_IncrementalMesh mesh( shape, stl_lin_tol);
	mesh.Perform();

	Face_vector faces;

	if ( (output==OUT_STL_ASCII) || (output==OUT_STL_SCAD) || (output==OUT_STL_FACES) )
		faces = tessellate_shape (shape);

	switch (output)
	{
	case OUT_STL_ASCII:
		write_triangles_ascii_stl(faces);
		break;

	case OUT_STL_SCAD:
		write_triangle_scad(faces);
		break;

	case OUT_STL_FACES:
		write_faces_scad(faces);
		break;

	case OUT_STL_OCCT:
		try
		{
			StlAPI_Writer writer;
			writer.Write(shape,"/dev/stdout");
		} catch(Standard_ConstructionError& e) {
			errx(1,"failed to write OCCT/STL: %s", e.GetMessageString());
		}
		break;

	case OUT_EXPLORE:
		explore_shape (shape);
		break;
	}

	return 0;
}
