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
#include <BRepPrimAPI_MakeCylinder.hxx>
#include <BRepPrimAPI_MakeCone.hxx>
#include <BRepPrimAPI_MakeBox.hxx>
#include <BRepPrimAPI_MakeSphere.hxx>
#include <BRepPrimAPI_MakePrism.hxx>
#include <BRepAlgoAPI_Cut.hxx>
#include <BRepAlgoAPI_Fuse.hxx>
#include <BRepAlgoAPI_Common.hxx>
#include <BRepBuilderAPI_Sewing.hxx>
#include <BRepBuilderAPI_MakeSolid.hxx>
#include <TopoDS_Shell.hxx>
#include <TopExp.hxx>
#include <TopoDS.hxx>
#include <ShapeFix_Shape.hxx>
#include <BRepGProp.hxx>
#include <STEPControl_Reader.hxx>
#include <STEPControl_Writer.hxx>
#include <GProp_GProps.hxx>
#include <IGESControl_Reader.hxx>
#include <IGESControl_Writer.hxx>
#include <BRepTools.hxx>
#include <BRep_Builder.hxx>
#include <StlAPI_Writer.hxx>
#include <BRepMesh_IncrementalMesh.hxx>
#include <RWStl.hxx>
#include <Poly_Triangulation.hxx>
#include <OSD_Path.hxx>
#include <TopoDS_Compound.hxx>
#include <TopoDS_Vertex.hxx>
#include <TopoDS_Wire.hxx>
#include <TopoDS_Face.hxx>
#include <Bnd_Box.hxx>
#include <BRepBndLib.hxx>
#include <BRepBuilderAPI_MakeVertex.hxx>
#include <BRepBuilderAPI_MakePolygon.hxx>
#include <BRepBuilderAPI_MakeFace.hxx>
#include <BRepBuilderAPI_MakeEdge.hxx>
#include <BRepBuilderAPI_MakeWire.hxx>
#include <gp_Circ.hxx>
#include <TopExp_Explorer.hxx>
#include <Font_BRepFont.hxx>
#include <Font_BRepTextBuilder.hxx>
#include <BRepBuilderAPI_GTransform.hxx>
#include <BRepBuilderAPI_Transform.hxx>
#include <gp_GTrsf.hxx>
#include <BRepFilletAPI_MakeFillet.hxx>
#include <BRepFilletAPI_MakeChamfer.hxx>
#include <TColgp_HArray1OfPnt.hxx>
#include <BRepTools_WireExplorer.hxx>
#include <math.hxx>


#include "triangle.h"
#include "tessellation.h"

Face tessellate_face(const TopoDS_Face &aFace)
{
	Face output_face;

	/* This code is based on
	   https://www.opencascade.com/content/how-get-triangles-vertices-data-absolute-coords-native-opengl-rendering
	*/
	TopAbs_Orientation faceOrientation = aFace.Orientation();

	TopLoc_Location aLocation;
	Handle(Poly_Triangulation) aTr = BRep_Tool::Triangulation(aFace,aLocation);

	if(!aTr.IsNull())
	{
		TColgp_Array1OfPnt aPoints(1, aTr->NbNodes());
		for(Standard_Integer i = 1; i < aPoints.Size()+1; i++)
			aPoints(i) = aTr->Node(i).Transformed(aLocation);

		Standard_Integer nnn = aTr->NbTriangles();
		Standard_Integer nt,n1,n2,n3;

		for( nt = 1 ; nt < nnn+1 ; nt++)
		{
			aTr->Triangle(nt).Get(n1,n2,n3);

			if (faceOrientation != TopAbs_Orientation::TopAbs_FORWARD)
			{
				int tmp = n1;
				n1 = n3;
				n3 = tmp;
			}

			gp_Pnt aPnt1 = aPoints(n1);
			gp_Pnt aPnt2 = aPoints(n2);
			gp_Pnt aPnt3 = aPoints(n3);

			const Triangle tr(aPnt1, aPnt2, aPnt3);
			output_face.addTriangle(tr);
		}
	}

	return output_face;
}


Face_vector tessellate_shape (const TopoDS_Shape& shape)
{
	Face_vector output_faces;

	for (TopExp_Explorer FaceExp(shape, TopAbs_FACE); FaceExp.More(); FaceExp.Next())
	{
		const TopoDS_Face &aFace = TopoDS::Face(FaceExp.Current());

		const Face &f = tessellate_face(aFace);
		output_faces.push_back(f);
	}

	return output_faces;
}
