#include <math.h>
#include "vector.h"

double Vector::r() {
	return sqrt(x*x + y*y + z*z);
}

double Vector::r2() {
	return x*x + y*y + z*z;
}

double Vector::fast_inverse() {
	
	float tmp = x*x + y*y + z*z;

	long i;
	float x2, a;
	const float threehalfs = 1.5F;
	x2 = tmp * 0.5F;
	a  = tmp;
	i  = * ( long * ) &a;
	i  = 0x5f3759df - ( i >> 1 );
	a  = * ( float * ) &i;
	a  = a * ( threehalfs - ( x2 * a * a ) );   // 1st iteration
	//a  = a * ( threehalfs - ( x2 * a * a ) );   // 2nd iteration, this can be removed

	return (double)a;
}

Vector Vector::norm(double R) {
	double _r = r();
	if ( _r == 0.0 ) {
		printf("Can't normalize vector with zero length!\n");
		return *this;
	}
	return (R/_r)*(*this);
}

Vector & Vector::operator=(double other) {
	x = other;
	y = other;
	z = other;
	return *this;
}

bool operator==(const Vector & f, const Vector & s) {
	return f.x == s.x &&  f.y == s.y &&  f.z == s.z;
}

Vector operator+(const Vector & f, const Vector & s) {
	return Vector(f.x + s.x, f.y + s.y, f.z + s.z);
}

Vector operator-(const Vector & f, const Vector & s) {
	return Vector(f.x - s.x, f.y - s.y, f.z - s.z);
}

double operator*(const Vector & f, const Vector & s) {
	return f.x * s.x + f.y * s.y + f.z * s.z;
}

Vector operator*(const Vector & f, const double & s) {
	return Vector(f.x * s, f.y * s, f.z * s);
}

Vector operator*(const double & f, const Vector & s) {
	return Vector(s.x * f, s.y * f, s.z * f);
}

void printf_vector(const Vector & v, const char * before, const char * after, const char * fmt) {
	
	if ( before != NULL )
		printf("%s", before);

	char buf[100];
	sprintf(buf, "<%s,%s,%s>", fmt, fmt, fmt);

	printf(buf, v.x, v.y, v.z);
	
	if ( after != NULL )
		printf("%s", after);

}
