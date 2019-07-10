#include <iostream>
#include "Vector3.h"



Vector3::Vector3():
	x(0.0), y(0.0), z(0.0)
{}

Vector3::Vector3(double const& ex, double const& ey, double const& ez):
	x(ex), y(ey), z(ez)
{}


Vector3::~Vector3() {
}


double Vector3::length() const{
	return sqrt(x*x + y*y + z*z);
}

void Vector3::reset(){
	x=0.0;
	y=0.0;
	z=0.0;
}

Vector3 operator+(Vector3 const& A, Vector3 const& B){
	return Vector3{A.x+B.x, A.y+B.y, A.z+B.z};
}

Vector3 operator*(double const& a, Vector3 const& B) {
	return Vector3 {a*B.x, a*B.y, a*B.z};
}

