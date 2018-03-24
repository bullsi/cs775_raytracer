#include <iostream>
#include <math.h>

#ifndef _UTIL_H_
#define _UTIL_H_

using namespace std;

const double maxIntensity = 255;
const double EPSILON = 1e-5;
const double INF = 32768;
const double PI = 3.142;

struct Vector3
{
	double x, y, z;
	double mag;
	Vector3(double x=0, double y=0, double z=1)
	{
		setValues(x,y,z);
	}
	Vector3(const Vector3& v)
	{
		x = v.x;
		y = v.y;
		z = v.z;
		mag = sqrt(x*x + y*y + z*z);
	}
	void setValues(double x, double y, double z)
	{
		this->x = x;
		this->y = y;
		this->z = z;
		mag = sqrt(x*x + y*y + z*z);
	}
	Vector3 clone()
	{
		Vector3 temp;
		temp.x = x;
		temp.y = y;
		temp.z = z;
		return temp;
	}
	void normalize()
	{
		x /= mag;
		y /= mag;
		z /= mag;
		mag = 1;
	}
	// -this
	Vector3 negate()
	{
		Vector3 temp;
		temp.x = -x;
		temp.y = -y;
		temp.z = -z;
		return temp;
	}
	// this + b
	Vector3 add(Vector3 b)
	{ 
		Vector3 temp;
		temp.x = x + b.x;
		temp.y = y + b.y;
		temp.z = z + b.z;
		return temp;
	}
	// this - b
	Vector3 subtract(Vector3 b)
	{ 
		Vector3 temp;
		temp.x = x - b.x;
		temp.y = y - b.y;
		temp.z = z - b.z;
		return temp;
	}
	// this x b
	Vector3 crossProduct(Vector3 b)
	{
		Vector3 temp;
		temp.x = y*b.z - z*b.y;
		temp.y = z*b.x - x*b.z;
		temp.z = x*b.y - y*b.x;
		return temp;
	}
	// this . b
	double dotProduct(Vector3 b)
	{ return x*b.x + y*b.y + z*b.z; }
	double magnitude()
	{ return sqrt(x*x + y*y + z*z); }
	double magnitude_sq()
	{ return x*x + y*y + z*z; }
	Vector3 rotate_x(double cos_x, double sin_x)	// x:I,y:R,z:T from z to y
	{
		Vector3 temp;
		temp.x = x;
		temp.y = y*cos_x + z*sin_x;
		temp.z = -y*sin_x + z*cos_x;
		return temp;
	}
	Vector3 rotate_y(double cos_y, double sin_y)	// x:T,y:I,z:R from x to z
	{
		Vector3 temp;
		temp.x = -z*sin_y + x*cos_y;
		temp.y = y;
		temp.z = z*cos_y + x*sin_y;
		return temp;
	}
	Vector3 rotate_z(double cos_z, double sin_z)	// x:R,y:T,z:I from y to x
	{
		Vector3 temp;
		temp.x = x*cos_z + y*sin_z;
		temp.y = -x*sin_z + y*cos_z;
		temp.z = z;
		return temp;
	}
	Vector3 translate(Vector3 translation)
	{
		Vector3 temp;
		temp.x = x + translation.x;
		temp.y = y + translation.y;
		temp.z = z + translation.z;
		return temp;
	}
	Vector3 scale(Vector3 scaling)
	{
		Vector3 temp;
		temp.x = x*scaling.x;
		temp.y = y*scaling.y;
		temp.z = z*scaling.z;
		return temp;
	}
	Vector3 scale(double scaling)
	{
		Vector3 temp;
		temp.x = x*scaling;
		temp.y = y*scaling;
		temp.z = z*scaling;
		return temp;
	}
	double cos(Vector3 b)
	{
		return dotProduct(b) / (magnitude() * b.magnitude());
	}
	bool equals(Vector3 b)
	{
		//~ show(); b.show();
		//~ cout << y << " == " << b.y << "  " << y-b.y << endl;
		//~ cout << "expr = " << (fabs(x-b.x)<EPSILON) << ", " << (fabs(y-b.y)<EPSILON) << "," << (fabs(z-b.z)<EPSILON) << endl;
		return ( (fabs(x-b.x)<EPSILON) && (fabs(y-b.y)<EPSILON) && (fabs(z-b.z)<EPSILON) );
	}
	bool isNull()
	{ return (z == -INF); }
	void show()
	{ cerr << x << "," << y << "," << z << endl; }
};
struct Intensity
{
	double R, G, B;
	Intensity()
	{
		R = G = B = 0;
	}
	Intensity(double R, double G, double B)
	{
		this->R = R;
		this->G = G;
		this->B = B;
	}
	Intensity clone()
	{
		Intensity temp;
		temp.R = R;
		temp.G = G;
		temp.B = B;
		return temp;
	}
	Intensity add(Intensity b)
	{
		Intensity temp;
		temp.R = R + b.R;
		temp.G = G + b.G;
		temp.B = B + b.B;
		return temp;
	}
	Intensity scale(float scaling)
	{
		Intensity temp;
		temp.R = R * scaling;
		temp.G = G * scaling;
		temp.B = B * scaling;
		return temp;
	}
	Intensity scale(Vector3 scaling)
	{
		Intensity temp;
		temp.R = R * scaling.x;
		temp.G = G * scaling.y;
		temp.B = B * scaling.z;
		return temp;
	}
	bool isNull()
	{ return (B == -1); }
	void show()
	{ cerr << R << "," << G << "," << B << endl; }
};
#endif
