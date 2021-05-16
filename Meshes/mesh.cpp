#include "mesh.h"


Triangle::Triangle(const Vector & p1, const Vector & p2, const Vector & p3) {
	set_vertices(p1, p2, p3);
}


void Triangle::set_vertices(const Vector & _p1, const Vector & _p2, const Vector & _p3) {
	
	v1 = _p1;
	v2 = _p2;
	v3 = _p3;

	S = area();
	bc = barycenter();
}


double Triangle::area() {

	Vector dr1 = v1 - v2;
	Vector dr2 = v2 - v3;
	
	double sp = dr1*dr2;
	double res = dr1.r2()*dr2.r2() - sp*sp;

	return 0.5*sqrt(res);
}


Triangle * mesh_sphere_equilateral_triangles(double R, int order, const Vector center, int type) {

	int n = 0;
	if ( type == 1 )
		n = 8;
	if ( type == 2 )
		n = 4;
	if ( type == 3 )
		n = 20;
	
	Triangle ** trs = new Triangle*[order + 1];
	for (int i = 0; i < order + 1; i ++ ) {
		trs[i] = new Triangle[n];
		n *= 4;
	}

	if ( type == 1 ) {

		Vector v1(   R, 0.0, 0.0);
		Vector v2( 0.0,   R, 0.0);
		Vector v3(  -R, 0.0, 0.0);
		Vector v4( 0.0,  -R, 0.0);
		Vector vt( 0.0, 0.0,   R);
		Vector vb( 0.0, 0.0,  -R);
		trs[0][0].set_vertices(v1,v2,vt);
		trs[0][1].set_vertices(v2,v3,vt);
		trs[0][2].set_vertices(v3,v4,vt);
		trs[0][3].set_vertices(v4,v1,vt);
		trs[0][4].set_vertices(v1,v2,vb);
		trs[0][5].set_vertices(v2,v3,vb);
		trs[0][6].set_vertices(v3,v4,vb);
		trs[0][7].set_vertices(v4,v1,vb);
	}

	if ( type == 2 ) {
		
		Vector v1( R*sqrt(8.0/9.0),  0,               -R/3.0);
		Vector v2(-R*sqrt(2.0/9.0),  R*sqrt(2.0/3.0), -R/3.0);
		Vector v3(-R*sqrt(2.0/9.0), -R*sqrt(2.0/3.0), -R/3.0);
		Vector vt( 0,                0,                R);
		trs[0][0].set_vertices(v1,v2,vt);
		trs[0][1].set_vertices(v2,v3,vt);
		trs[0][2].set_vertices(v3,v1,vt);
		trs[0][3].set_vertices(v1,v2,v3);
	}

	if ( type == 3 ) {

		double gr = 0.5*(1.0 + sqrt(5.0));
		double nr = 1.0/sqrt(gr*gr + 1.0);
		gr *= nr;
		gr *= R;
		nr *= R;

		Vector vs[] = {Vector( 0 ,  nr,  gr),
					   Vector( 0 ,  nr, -gr),
					   Vector( 0 , -nr,  gr),
					   Vector( 0 , -nr, -gr),
					   Vector( nr,  gr,  0 ),
					   Vector( nr, -gr,  0 ),
					   Vector(-nr,  gr,  0 ),
					   Vector(-nr, -gr,  0 ),
					   Vector( gr,  0 ,  nr),
					   Vector( gr,  0 , -nr),
					   Vector(-gr,  0 ,  nr),
					   Vector(-gr,  0 , -nr)};

		double _4nr = 4*nr*nr;
		int c = 0;
		for ( int p1 = 0; p1 < 12; p1 ++ )
			for ( int p2 = p1 + 1; p2 < 12; p2 ++ ) {
				if ( fabs((vs[p1] - vs[p2]).r2() - _4nr) > 0.01*_4nr )
					continue;
				for ( int p3 = p2 + 1; p3 < 12; p3 ++ ) {
					if ( fabs((vs[p2] - vs[p3]).r2() - _4nr) > 0.01*_4nr or
						 fabs((vs[p3] - vs[p1]).r2() - _4nr) > 0.01*_4nr )
						continue;
					trs[0][c++].set_vertices(vs[p1],vs[p2],vs[p3]);
				}
			}

	}

	if ( type == 1 )
		n = 8;
	if ( type == 2 )
		n = 4;
	if ( type == 3 )
		n = 20;

	for (int i = 1; i < order + 1; i ++ ) {
		int c = 0;
		for (int t = 0; t < n; t ++ ) {
			Triangle * tr = trs[i-1] + t;
			Vector v1 = tr->get_v1();
			Vector v2 = tr->get_v2();
			Vector v3 = tr->get_v3();

			Vector m12 = (0.5*(v1 + v2)).norm(R);
			Vector m23 = (0.5*(v2 + v3)).norm(R);
			Vector m31 = (0.5*(v3 + v1)).norm(R);
			
			trs[i][c++].set_vertices(m31,v1,m12);
			trs[i][c++].set_vertices(m12,v2,m23);
			trs[i][c++].set_vertices(m23,v3,m31);
			trs[i][c++].set_vertices(m12,m23,m31);
		}
		n *= 4;
	}

	Triangle * res = trs[order];
	for (int i = 0; i < order; i ++ )
		delete[] trs[i];
	delete[] trs;
	return res;
}
