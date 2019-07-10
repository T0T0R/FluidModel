#pragma once

class Vector3
{
public:
	Vector3();
	Vector3(double const& ex, double const& ey, double const& ez);
	~Vector3();

	double length() const;
	void reset();

	double x;
	double y;
	double z;
};

Vector3 operator+(Vector3 const& A, Vector3 const& B);
Vector3 operator*(double const& a, Vector3 const& B);