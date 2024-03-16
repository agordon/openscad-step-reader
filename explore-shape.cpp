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
#include <BRepBuilderAPI_MakeSolid.hxx>
#include <TopoDS_Shell.hxx>
#include <TopExp.hxx>
#include <TopoDS.hxx>
#include <ShapeFix_Shape.hxx>
#include <BRepGProp.hxx>
#include <GProp_GProps.hxx>
#include <BRepTools.hxx>
#include <BRep_Builder.hxx>
#include <BRepMesh_IncrementalMesh.hxx>
#include <TopoDS_Compound.hxx>
#include <TopoDS_Vertex.hxx>
#include <TopoDS_Wire.hxx>
#include <TopoDS_Face.hxx>
#include <Bnd_Box.hxx>
#include <BRepBndLib.hxx>
#include <gp_Circ.hxx>
#include <TopExp_Explorer.hxx>
#include <gp_GTrsf.hxx>
#include <TColgp_HArray1OfPnt.hxx>
#include <BRepTools_WireExplorer.hxx>
#include <math.hxx>
#include <GeomAbs_SurfaceType.hxx>
#include <BRepAdaptor_Surface.hxx>


/* good resources:
   https://github.com/miho/OCC-CSG
   https://github.com/lvk88/OccTutorial
   https://dev.opencascade.org/doc/overview/html/index.html#OCCT_OVW_SECTION_7

*/

std::string SurfaceTypeName ( enum  	GeomAbs_SurfaceType t )
{
	switch (t)
	{
	case GeomAbs_Plane:
		return "Plane";
	case GeomAbs_Cylinder:
		return "Cylinder";
	case GeomAbs_Cone:
		return "Cone";
	case GeomAbs_Sphere:
		return "Sphere";
	case GeomAbs_Torus:
		return "Torus";
	case GeomAbs_BezierSurface:
		return "Bezier";
	case GeomAbs_BSplineSurface:
		return "BSplineSurface";
	case GeomAbs_SurfaceOfRevolution:
		return "SurfaceOfRevolution";
	case GeomAbs_SurfaceOfExtrusion:
		return "SurfaceOfExtrusion";
	case GeomAbs_OffsetSurface:
		return "OffsetSurface";
	case GeomAbs_OtherSurface:
		return "OtherSurface";
	}
	return "UNKNOWN";
}

std::string OrientationName (enum TopAbs_Orientation o)
{
	switch (o)
	{
	case TopAbs_FORWARD:
		return "FORWARD";
	case TopAbs_REVERSED:
		return "REVERSED";
	case TopAbs_INTERNAL:
		return "INTERNAL";
	case TopAbs_EXTERNAL:
		return "EXTERNAL";
	}
	return "UNKNOWN";
}



void get_vertex_points_test1 (const TopoDS_Shape& shape)
{
	// from: https://www.opencascade.com/content/extracting-boundary-topodsshape
	TopTools_IndexedMapOfShape vertices;
	TopExp::MapShapes(shape,TopAbs_VERTEX,vertices);

	Handle_TColgp_HArray1OfPnt result = new TColgp_HArray1OfPnt(1,vertices.Extent());

	for(Standard_Integer i = 1;i<=vertices.Extent();i++)
	{
		TopoDS_Vertex vertex = TopoDS::Vertex(vertices.FindKey(i));
		gp_Pnt currentGeometricPoint = BRep_Tool::Pnt(vertex);
		result->SetValue(i,currentGeometricPoint);
	}

	//Convert to array
	//See:  https://github.com/lvk88/OccTutorial/blob/master/Chapter1_Basics/src/WriteCoordinatesToFile.cpp
	const TColgp_Array1OfPnt& points = result->Array1();

	for(Standard_Integer i = points.Lower();i <= points.Upper();i++)
	{
		std::cout << "  vertex " << i << ": " << points.Value(i).X() << " " << points.Value(i).Y() << " " << points.Value(i).Z() << std::endl;

		auto &p = points.Value(i);
	}
}



void get_vertex_points_test2 (const TopoDS_Shape& shape)
{
	//from: https://www.opencascade.com/content/how-get-wires-single-face
	int i = 0;
	for (TopExp_Explorer anExp(shape, TopAbs_EDGE); anExp.More(); anExp.Next())
	{
		// Iterate over edges in input shape.
		const TopoDS_Edge& anEdge = TopoDS::Edge(anExp.Current());

		std::cout << "Edge " << i << " orientatino = " << OrientationName(anEdge.Orientation()) << std::endl;

		// Take the first and the last vertices from edge
		TopoDS_Vertex aVFirst = TopExp::FirstVertex(anEdge);
		TopoDS_Vertex aVLast  = TopExp::LastVertex(anEdge);

		// Take geometrical information from vertices.
		gp_Pnt aPntFirst = BRep_Tool::Pnt(aVFirst);
		gp_Pnt aPntLast = BRep_Tool::Pnt(aVLast);

		if (anEdge.Orientation() == TopAbs_FORWARD)
		{
		}
		else
		{
		}
		++i;
	}

}


void get_vertex_points_test3 (const TopoDS_Shape& shape)
{
	//from: https://www.opencascade.com/content/how-get-wires-single-face
	int i = 0;
	for (TopExp_Explorer anExp(shape, TopAbs_VERTEX); anExp.More(); anExp.Next())
	{
		const TopoDS_Vertex& anVertex = TopoDS::Vertex(anExp.Current());

		std::cout << "Vertex " << i << " orientatino = " << OrientationName(anVertex.Orientation()) << std::endl;

		// Take geometrical information from vertices.
		gp_Pnt aPnt = BRep_Tool::Pnt(anVertex);

		++i;
	}
}


void get_wire_edge_points_test4(const TopoDS_Shape& shape)
{
	int wire_idx = 0;
	for (TopExp_Explorer WireExp(shape, TopAbs_WIRE); WireExp.More(); WireExp.Next())
	{
		const TopoDS_Wire& aWire = TopoDS::Wire(WireExp.Current());
		std::cout << "    Wire " << wire_idx << std::endl;

		int edge_idx = 0;
		for (TopExp_Explorer EdgeExp(aWire, TopAbs_EDGE); EdgeExp.More(); EdgeExp.Next())
		{
			std::cout << "     edge " << edge_idx << std::endl;

			const TopoDS_Edge& anEdge = TopoDS::Edge(EdgeExp.Current());

			// Take the first and the last vertices from edge
			TopoDS_Vertex aVFirst = TopExp::FirstVertex(anEdge);
			TopoDS_Vertex aVLast  = TopExp::LastVertex(anEdge);

			// Take geometrical information from vertices.
			gp_Pnt aPntFirst = BRep_Tool::Pnt(aVFirst);
			gp_Pnt aPntLast = BRep_Tool::Pnt(aVLast);


			std::cout << "       VertexFirst " << aPntFirst.X() << ", "
				  << aPntFirst.Y() << ", "
				  << aPntFirst.Z() << std::endl;
			std::cout << "       VertexLast " << aPntLast.X() << ", "
				  << aPntLast.Y() << ", "
				  << aPntLast.Z() << std::endl;

			++edge_idx;
		}
		++wire_idx;
	}
}

TopoDS_Shape make_solid(const TopoDS_Shape& shape)
{
	BRepBuilderAPI_MakeSolid solidmaker;
	int i = 0;
	for (TopExp_Explorer fExpl(shape, TopAbs_SHELL); fExpl.More(); fExpl.Next())
	{
		const TopoDS_Shell &curShell = TopoDS::Shell(fExpl.Current());
		++i;

		//debugGetVerticesOfShape(curShell);
		solidmaker.Add(curShell);
	}
	std::cout << std::endl;
	TopoDS_Shape solid = solidmaker.Solid();

	return solid;
}


void explore_shape(const TopoDS_Shape& shape)
{
	int shell_i = 0;
	for (TopExp_Explorer ShellExp(shape, TopAbs_SHELL); ShellExp.More(); ShellExp.Next())
	{
		const TopoDS_Shell &curShell = TopoDS::Shell(ShellExp.Current());

		std::cout << "Shell " << shell_i << std::endl;

		int i = 0;
		for (TopExp_Explorer FaceExp(curShell, TopAbs_FACE); FaceExp.More(); FaceExp.Next())
		{
			const TopoDS_Face &aFace = TopoDS::Face(FaceExp.Current());

			//from: https://www.opencascade.com/content/how-determine-whether-topodsface-flat-or-volumetric
			BRepAdaptor_Surface anAdaptor(aFace);
			std::cout << "  Face " << i << " Type = " << anAdaptor.GetType() << ":" << SurfaceTypeName(anAdaptor.GetType()) << std::endl;

			get_wire_edge_points_test4(aFace);
		}
	}
}
