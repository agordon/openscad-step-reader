std::string SurfaceTypeName ( enum GeomAbs_SurfaceType t )
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
