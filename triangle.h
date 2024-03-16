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
#ifndef __TRIANGLE__
#define __TRIANGLE__

class Point {
	double _x,_y,_z;
public:
	Point() : _x(0), _y(0), _z(0) {};
	Point(double x, double y, double z):_x(x),_y(y),_z(z) {};
	Point(const gp_Pnt& p) : _x(p.X()),_y(p.Y()),_z(p.Z()) {} ;
	double x() const { return _x; };
	double y() const { return _y; };
	double z() const { return _z; };

	void write_ascii_stl(std::ostream &ostrm) const
		{
			ostrm << "vertex " << _x << " " << _y << " " << _z ;
		}

};
static std::ostream & operator << (std::ostream &out, const Point &p)
{
	out << "[" << p.x() << "," << p.y() << "," << p.z() << "]";
	return out;
}


class Triangle {
	Point _p1, _p2, _p3;
public:
	Triangle() {} ;
	Triangle(const Point& p1, const Point& p2, const Point& p3) : _p1(p1), _p2(p2), _p3(p3) {}

	void write_points_vector(std::ostream &ostrm) const
		{
			ostrm << _p1 << "," << _p2 << "," << _p3 ;
		}

	void write_ascii_stl(std::ostream &ostrm) const
		{
			ostrm << "    " ;
			_p1.write_ascii_stl(ostrm);
			ostrm << std::endl;

			ostrm << "    " ;
			_p2.write_ascii_stl(ostrm);
			ostrm << std::endl;

			ostrm << "    " ;
			_p3.write_ascii_stl(ostrm);
			ostrm << std::endl;
		}

};



class Face {
	std::vector<Triangle> triangles;
public:
	Face() {};
	void addTriangle(const Triangle& tr) { triangles.push_back(tr); };

	void add_face(const Face& other_face)
		{
			triangles.insert(triangles.end(),
					 other_face.triangles.begin(),
					 other_face.triangles.end());
		}

	void write_ascii_stl(std::ostream &ostrm) const
		{
			for (auto &t : triangles) {
				ostrm << " facet normal 42 42 42" << std::endl;
				ostrm << "   outer loop" << std::endl;
				t.write_ascii_stl(ostrm);
				ostrm << "   endloop" << std::endl;
				ostrm << " endfacet" << std::endl;
			}
		}

	void write_points_vector(std::ostream &ostrm) const
		{
			int i = 1 ;
			ostrm << "[" << std::endl;
			for (auto &t : triangles) {
				ostrm << "  ";
				t.write_points_vector(ostrm);
				ostrm << ",";
				if (i==1 || (i%10==0 && triangles.size()>10))
					ostrm << " // Triangle " << i << " / " << triangles.size();
				ostrm << std::endl;
				++i;
			}
			ostrm << "];" << std::endl;
		}

	void write_face_vector(std::ostream &ostrm) const
		{
			ostrm << "[" << std::endl;
			for (int i=0;i<triangles.size();++i) {
				int idx = i*3;
				ostrm << "  [" << idx << "," << (idx+1) << "," << (idx+2) << "],";
				if (i==0 || ((i+1)%10==0 && triangles.size()>10))
					ostrm << " // Triangle " << (i+1) << " / " << triangles.size();
				ostrm << std::endl;
			}
			ostrm << "];" << std::endl;
		}
};
typedef std::vector<Face> Face_vector;


#endif
