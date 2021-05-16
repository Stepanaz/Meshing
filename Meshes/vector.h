#ifndef __VECTOR_H__
#define __VECTOR_H__

#include <stdio.h>

class Vector {
	public:
		Vector(double x = 0, double y = 0, double z = 0): x(x), y(y), z(z) {};
		
		double x;
		double y;
		double z;

		double r();
		double r2();
		double fast_inverse();
		Vector norm(double R = 1.0);
		Vector & operator=(double other);
};

bool   operator==(const Vector & f, const Vector & s);
Vector operator+ (const Vector & f, const Vector & s);
Vector operator- (const Vector & f, const Vector & s);
double operator* (const Vector & f, const Vector & s);
Vector operator* (const Vector & f, const double & s);
Vector operator* (const double & f, const Vector & s);

void printf_vector(const Vector & v, const char * before = NULL, const char * after = NULL, const char * fmt = "%.4g");

#endif
