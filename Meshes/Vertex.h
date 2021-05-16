#ifndef __VERTEX_H__
#define __VERTEX_H__

#include <stdio.h>
#include "vector.h"
#include <list>
#include "mesh.h"

using namespace std;
class Vertex;
class Halfedge;
class mesh;

class Vertex: public Vector {
public:
	//int incedent_amount количество связанных вершин.
	list<Vertex> incedent_Vertices;
	list<Halfedge> incedent_Halfedges;
	list<Triangle> Connected_triangles;
	Vertex()
	{
		x = 0;
		y = 0;
		z = 0;
	}

	Vertex(Vector A)
	{
		x = A.x;
		y = A.y;
		z = A.z;
	}
};

class Halfedge : public Vector {
public:
	Vertex *next;
	
	Halfedge()
	{
		x = 0;
		y = 0;
		z = 0;
	}
	Halfedge(Vector A)
	{
		x = A.x;
		y = A.y;
		z = A.z;
	}
};

class mesh {
public:
	list<Vertex> Vertices;
	list<Triangle> Triangles;
};
#endif