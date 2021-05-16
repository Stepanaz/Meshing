#ifndef __MESH_H__
#define __MESH_H__

#include <math.h>
#include "vector.h"

class Triangle {
	public:

		Triangle(const Vector & v1 = Vector(0,0,0), const Vector & v2 = Vector(0,0,0), const Vector & v3 = Vector(0,0,0));

		void set_vertices(const Vector & v1, const Vector & v2, const Vector & v3);
		inline Vector get_barycenter() { return bc; };
		inline double get_area() { return S; };

		Vector get_v1() { return v1; };
		Vector get_v2() { return v2; };
		Vector get_v3() { return v3; };
	
	private:
	
		Vector v1;
		Vector v2;
		Vector v3;

		Vector bc; // Barycenter value
		double S; 	// Aria value

		double area();
		inline Vector barycenter() { return 0.333333333333*(v1 + v2 + v3); };
};

// type == 1 Octahedron
// type == 2 Tetrahedron
Triangle * mesh_sphere_equilateral_triangles(double R, int order = 3, const Vector center = Vector(0,0,0), int type = 1);

#endif
