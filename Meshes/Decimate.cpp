#include <cassert>
#include "Decimate.h"
#include "Array3D.h"
#include "math3D.h"
#include <iostream>

static inline void add_triangle(mesh& setka, Vector3D A, Vector3D B, Vector3D C)
{
	Triangle ABC;
	Vector one(A.x, A.y, A.z);
	Vector two(B.x, B.y, B.z);
	Vector three(C.x, C.y, C.z);
	ABC.set_vertices(one, two, three);

	Vertex A1(one);//заводим вершины
	Vertex B1(two);
	Vertex C1(three);
	
	A1.incedent_Vertices.push_back(B1);//добавляем в вершины указатели на инцедентные
	A1.incedent_Vertices.push_back(C1);

	B1.incedent_Vertices.push_back(A1);
	B1.incedent_Vertices.push_back(C1);

	C1.incedent_Vertices.push_back(A1);
	C1.incedent_Vertices.push_back(B1);

	Halfedge ab(one);//заводим полурёбра
	Halfedge ac(one);

	Halfedge ba(two);
	Halfedge bc(two);

	Halfedge ca(three);
	Halfedge cb(three);

	ab.next = &B1;//даём полурёбрам указатели на следующие вершины
	ac.next = &C1;

	ba.next = &A1;
	bc.next = &C1;

	ca.next = &A1;
	cb.next = &B1;

	A1.incedent_Halfedges.push_back(ab);//даём вершинам ссылки на полурёбра
	A1.incedent_Halfedges.push_back(ac);

	B1.incedent_Halfedges.push_back(ba);
	B1.incedent_Halfedges.push_back(bc);

	C1.incedent_Halfedges.push_back(ca);
	C1.incedent_Halfedges.push_back(cb);

	A1.Connected_triangles.push_back(ABC);// в каждую вершину даём ссылку на текущий треугольник
	B1.Connected_triangles.push_back(ABC);
	C1.Connected_triangles.push_back(ABC);
	
	setka.Triangles.push_back(ABC);// сразу добваляем в сетку треугольник, т.к. метод подразумевает что треугольники в тетраэдрах и кубах не совпадут

	if (setka.Vertices.empty()) //на случай если сетка ещё пустая
	{
		setka.Vertices.push_back(A1);
		setka.Vertices.push_back(B1);
		setka.Vertices.push_back(C1);
	}
	else
	{
		//нам нужно пройтись по всей сетке и определить, есть ли в ней уже вершина с такими же координатами
		//возникает зависимость времени выполнения O(N^2), но её можно исправить через 3D массив списков вершин в каждом кубе, через который
		//можно было бы проверять существование вершины в сетке, но мне пока лень это писать

		list <Vertex>::iterator Iter;  
		Iter = setka.Vertices.begin();

		Vertex tmp;
		bool vertex_exists = false;
		
		

		for (Iter = setka.Vertices.begin(); Iter != setka.Vertices.end(); Iter++)//сначала проверим, есть ли вершина А1 в сетке
		{
			tmp = *Iter;
			if ((tmp.x == A1.x) && (tmp.y == A1.y) && (tmp.z == A1.z))
			{
				tmp.incedent_Vertices.push_back(A1.incedent_Vertices.front());
				tmp.incedent_Vertices.push_back(A1.incedent_Vertices.back());

				tmp.incedent_Halfedges.push_back(A1.incedent_Halfedges.front());
				tmp.incedent_Halfedges.push_back(A1.incedent_Halfedges.back());

				tmp.Connected_triangles.push_back(ABC);
				//tmp.incedent_Vertices.merge(A1.incedent_Vertices);
				//tmp.incedent_Halfedges.merge(A1.incedent_Halfedges);
				//tmp.Connected_triangles.merge(A1.Connected_triangles);
				Iter = setka.Vertices.erase(Iter);
				setka.Vertices.push_back(tmp);
				vertex_exists = true;
				break;
			}
		}
		if (!vertex_exists) setka.Vertices.push_back(A1); //если такой вершины ещё не было, то добавим её
		vertex_exists = false;
		for (Iter = setka.Vertices.begin(); Iter != setka.Vertices.end(); Iter++)//теперь B1
		{
			tmp = *Iter;
			if ((tmp.x == B1.x) && (tmp.y == B1.y) && (tmp.z == B1.z))
			{
				//tmp.incedent_Vertices.merge(B1.incedent_Vertices);
				//tmp.incedent_Halfedges.merge(B1.incedent_Halfedges);
				//tmp.Connected_triangles.merge(B1.Connected_triangles);
				tmp.incedent_Vertices.push_back(B1.incedent_Vertices.front());
				tmp.incedent_Vertices.push_back(B1.incedent_Vertices.back());

				tmp.incedent_Halfedges.push_back(B1.incedent_Halfedges.front());
				tmp.incedent_Halfedges.push_back(B1.incedent_Halfedges.back());

				tmp.Connected_triangles.push_back(ABC);

				Iter = setka.Vertices.erase(Iter);
				setka.Vertices.push_back(tmp);
				vertex_exists = true;
				break;
			}
		}
		if (!vertex_exists) setka.Vertices.push_back(B1); //если такой вершины ещё не было, то добавим её
		vertex_exists = false;
		for (Iter = setka.Vertices.begin(); Iter != setka.Vertices.end(); Iter++)//ну и напоследок C1
		{
			tmp = *Iter;
			if ((tmp.x == C1.x) && (tmp.y == C1.y) && (tmp.z == C1.z))
			{
				//tmp.incedent_Vertices.merge(C1.incedent_Vertices);
				//tmp.incedent_Halfedges.merge(C1.incedent_Halfedges);
				//tmp.Connected_triangles.merge(C1.Connected_triangles);
				tmp.incedent_Vertices.push_back(C1.incedent_Vertices.front());
				tmp.incedent_Vertices.push_back(C1.incedent_Vertices.back());

				tmp.incedent_Halfedges.push_back(C1.incedent_Halfedges.front());
				tmp.incedent_Halfedges.push_back(C1.incedent_Halfedges.back());

				tmp.Connected_triangles.push_back(ABC);

				Iter = setka.Vertices.erase(Iter);
				setka.Vertices.push_back(tmp);
				vertex_exists = true;
				break;
			}
		}
		if (!vertex_exists) setka.Vertices.push_back(C1); //если такой вершины ещё не было, то добавим её
		vertex_exists = false;
	}
}


static inline Vector3D drawVert(const Isosurface& surface, const Point3D& p1, const Point3D& p2, float isolevel)
{

	float v1 = p1.value;
	float v2 = p2.value;

	float x, y, z;

	if (v2 == v1) {
		x = (p1.x + p2.x) / 2.0f;
		y = (p1.y + p2.y) / 2.0f;
		z = (p1.z + p2.z) / 2.0f;
	}
	else {

		/*

		 <----+-----+---+----->
			  v1    |   v2
				 isolevel


		 <----+-----+---+----->
			  0     |   1
				  interp

		 */


		 // interp == 0: vert should be at p1
		 // interp == 1: vert should be at p2
		float interp = (isolevel - v1) / (v2 - v1);
		float oneMinusInterp = 1 - interp;

		x = p1.x * oneMinusInterp + p2.x * interp;
		y = p1.y * oneMinusInterp + p2.y * interp;
		z = p1.z * oneMinusInterp + p2.z * interp;
	}

	Vector3D res;
	res.x = x;
	res.y = y;
	res.z = z;
	return res;
	//Vector3D normal = surface.gradientAt(x, y, z);

	//glNormal3f(normal.x, normal.y, normal.z);
	//glVertex3f(x, y, z);
}

static void drawTetrahedron(mesh& setka, const Isosurface& surface, const Point3D p[4], float isolevel)
{

	Vector3D A, B, C, A1, B1, C1; // структуры вектор3д для вершин треугольников

	/*

	 Tetrahedron layout:

		   0
		   *
		  /|\
		 / | \
	  3 *-----* 1
		 \ | /
		  \|/
		   *
		   2
	 */

	unsigned char index = 0;
	for (int i = 0; i < 4; ++i)
		if (p[i].value < isolevel)
			index |= (1 << i);

	switch (index) {

		// we don't do anything if everyone is inside or outside
	case 0x00:
	case 0x0F:
		break;

		// only vert 0 is inside
	case 0x01:
		A = drawVert(surface, p[0], p[1], isolevel);
		B = drawVert(surface, p[0], p[3], isolevel);
		C = drawVert(surface, p[0], p[2], isolevel);
		add_triangle(setka, A, B, C);
		break;

		// only vert 1 is inside
	case 0x02:
		A = drawVert(surface, p[1], p[0], isolevel);
		B = drawVert(surface, p[1], p[2], isolevel);
		C = drawVert(surface, p[1], p[3], isolevel);
		add_triangle(setka, A, B, C);
		break;

		// only vert 2 is inside
	case 0x04:
		A = drawVert(surface, p[2], p[0], isolevel);
		B = drawVert(surface, p[2], p[3], isolevel);
		C = drawVert(surface, p[2], p[1], isolevel);
		add_triangle(setka, A, B, C);
		break;

		// only vert 3 is inside
	case 0x08:
		A = drawVert(surface, p[3], p[1], isolevel);
		B = drawVert(surface, p[3], p[2], isolevel);
		C = drawVert(surface, p[3], p[0], isolevel);
		add_triangle(setka, A, B, C);
		break;

		// verts 0, 1 are inside
	case 0x03:
		A = drawVert(surface, p[3], p[0], isolevel);
		B = drawVert(surface, p[2], p[0], isolevel);
		C = drawVert(surface, p[1], p[3], isolevel);
		add_triangle(setka, A, B, C);

		A1 = drawVert(surface, p[2], p[0], isolevel);
		B1 = drawVert(surface, p[2], p[1], isolevel);
		C1 = drawVert(surface, p[1], p[3], isolevel);
		add_triangle(setka, A1, B1, C1);
		break;

		// verts 0, 2 are inside
	case 0x05:
		A = drawVert(surface, p[3], p[0], isolevel);
		B = drawVert(surface, p[1], p[2], isolevel);
		C = drawVert(surface, p[1], p[0], isolevel);
		add_triangle(setka, A, B, C);

		A1 = drawVert(surface, p[1], p[2], isolevel);
		B1 = drawVert(surface, p[3], p[0], isolevel);
		C1 = drawVert(surface, p[2], p[3], isolevel);
		add_triangle(setka, A1, B1, C1);
		break;

		// verts 0, 3 are inside
	case 0x09:
		A = drawVert(surface, p[0], p[1], isolevel);
		B = drawVert(surface, p[1], p[3], isolevel);
		C = drawVert(surface, p[0], p[2], isolevel);
		add_triangle(setka, A, B, C);

		A1 = drawVert(surface, p[1], p[3], isolevel);
		B1 = drawVert(surface, p[3], p[2], isolevel);
		C1 = drawVert(surface, p[0], p[2], isolevel);
		add_triangle(setka, A1, B1, C1);
		break;

		// verts 1, 2 are inside
	case 0x06:
		A = drawVert(surface, p[0], p[1], isolevel);
		B = drawVert(surface, p[0], p[2], isolevel);
		C = drawVert(surface, p[1], p[3], isolevel);
		add_triangle(setka, A, B, C);

		A1 = drawVert(surface, p[1], p[3], isolevel);
		B1 = drawVert(surface, p[0], p[2], isolevel);
		C1 = drawVert(surface, p[3], p[2], isolevel);
		add_triangle(setka, A1, B1, C1);
		break;

		// verts 2, 3 are inside
	case 0x0C:
		A = drawVert(surface, p[1], p[3], isolevel);
		B = drawVert(surface, p[2], p[0], isolevel);
		C = drawVert(surface, p[3], p[0], isolevel);
		add_triangle(setka, A, B, C);

		A1 = drawVert(surface, p[2], p[0], isolevel);
		B1 = drawVert(surface, p[1], p[3], isolevel);
		C1= drawVert(surface, p[2], p[1], isolevel);
		add_triangle(setka, A1, B1, C1);
		break;

		// verts 1, 3 are inside
	case 0x0A:
		A = drawVert(surface, p[3], p[0], isolevel);
		B = drawVert(surface, p[1], p[0], isolevel);
		C = drawVert(surface, p[1], p[2], isolevel);
		add_triangle(setka, A, B, C);

		A1 = drawVert(surface, p[1], p[2], isolevel);
		B1 = drawVert(surface, p[2], p[3], isolevel);
		C1 = drawVert(surface, p[3], p[0], isolevel);
		add_triangle(setka, A1, B1, C1);
		break;

		// verts 0, 1, 2 are inside
	case 0x07:
		A = drawVert(surface, p[3], p[0], isolevel);
		B = drawVert(surface, p[3], p[2], isolevel);
		C = drawVert(surface, p[3], p[1], isolevel);
		add_triangle(setka, A, B, C);
		break;

		// verts 0, 1, 3 are inside
	case 0x0B:
		A = drawVert(surface, p[2], p[1], isolevel);
		B = drawVert(surface, p[2], p[3], isolevel);
		C = drawVert(surface, p[2], p[0], isolevel);
		add_triangle(setka, A, B, C);
		break;

		// verts 0, 2, 3 are inside
	case 0x0D:
		A = drawVert(surface, p[1], p[0], isolevel);
		B = drawVert(surface, p[1], p[3], isolevel);
		C = drawVert(surface, p[1], p[2], isolevel);
		add_triangle(setka, A, B, C);
		break;

		// verts 1, 2, 3 are inside
	case 0x0E:
		A = drawVert(surface, p[0], p[1], isolevel);
		B = drawVert(surface, p[0], p[2], isolevel);
		C = drawVert(surface, p[0], p[3], isolevel);
		add_triangle(setka, A, B, C);
		break;

		// what is this I don't even
	default:
		assert(false);
	}

}

void decimate(mesh& setka,  const Isosurface& surface,
	float xMin, float xMax,
	float yMin, float yMax,
	float zMin, float zMax,
	float isolevel,
	std::size_t resolution) // resolution indicates # of cubes
{

	std::size_t pointRes = resolution + 1; // indicates the # of points per side

	float xrange = xMax - xMin;
	float yrange = yMax - yMin;
	float zrange = zMax - zMin;

	Array3D<float> grid(pointRes, pointRes, pointRes);

	for (std::size_t i = 0; i <= resolution; ++i) {
		float x = (float)i / resolution * xrange + xMin;
		for (std::size_t j = 0; j <= resolution; ++j) {
			float y = (float)j / resolution * yrange + yMin;
			for (std::size_t k = 0; k <= resolution; ++k) {
				float z = (float)k / resolution * zrange + zMin;
				float value = surface.valueAt(x, y, z);
				grid(i, j, k) = value;
			}
		}
	}


	
	for (std::size_t i = 0; i < resolution; ++i) {
		cout << i << "\n";
		float x1 = (float)i / resolution * xrange + xMin;
		float x2 = (float)(i + 1) / resolution * xrange + xMin;
		for (std::size_t j = 0; j < resolution; ++j) {
			float y1 = (float)j / resolution * yrange + yMin;
			float y2 = (float)(j + 1) / resolution * yrange + yMin;
			for (std::size_t k = 0; k < resolution; ++k) {
				float z1 = (float)k / resolution * zrange + zMin;
				float z2 = (float)(k + 1) / resolution * zrange + zMin;

				/*

				 Coordinates:

					  z
					  |
					  |___ y
					  /
					 /
					x

				 Cube layout:

					4-------7
				   /|      /|
				  / |     / |
				 5-------6  |
				 |  0----|--3
				 | /     | /
				 |/      |/
				 1-------2


				 Tetrahedrons are:
					 0, 7, 3, 2
					 0, 7, 2, 6
					 0, 4, 6, 7
					 0, 6, 1, 2
					 0, 6, 1, 4
					 5, 6, 1, 4

				 */

				const Point3D v[8] = {
					{x1, y1, z1, grid(i,     j,     k)},
					{x2, y1, z1, grid(i + 1, j,     k)},
					{x2, y2, z1, grid(i + 1, j + 1, k)},
					{x1, y2, z1, grid(i,     j + 1, k)},
					{x1, y1, z2, grid(i,     j,     k + 1)},
					{x2, y1, z2, grid(i + 1, j,     k + 1)},
					{x2, y2, z2, grid(i + 1, j + 1, k + 1)},
					{x1, y2, z2, grid(i,     j + 1, k + 1)}
				};

				const Point3D tetrahedra[6][4] = {
					{ v[0], v[7], v[3], v[2] },
					{ v[0], v[7], v[2], v[6] },
					{ v[0], v[4], v[7], v[6] },
					{ v[0], v[1], v[6], v[2] },
					{ v[0], v[4], v[6], v[1] },
					{ v[5], v[1], v[6], v[4] }
				};

				for (int t = 0; t < 6; ++t)
					drawTetrahedron(setka, surface, tetrahedra[t], isolevel);

			}
		}
	}
	

}
