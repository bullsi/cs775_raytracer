#include <iostream>
#include "my_utilities.h"
#include "object.h"
#include "ray.h"

using namespace std;

// An equilateral triangle of side 2, centered at middle of the base
class Triangle : public Object
{
	Vector3 v[3];
	public:
		Vector3 normal;
		
		Triangle(Vector3 v1,
			Vector3 v2,
			Vector3 v3,
			Vector3 reflectivity = Vector3(1,1,1),
			double mirrorNess = 0.0,
			double opacity = 1,
			Intensity ambient = Intensity(50,50,50),
			Vector3 weights = Vector3(1,0,0),
			double shininess = 30,
			double refractiveIndex = 0.7
		) : Object(Vector3(0,0,0), Vector3(1,1,1), Vector3(0,0,0), reflectivity, mirrorNess, opacity, weights, ambient, shininess, refractiveIndex)
		{
			v[0] = v1;
			v[1] = v2;
			v[2] = v3;
			// If v1,v2,v3 are arranged in anti-clockwise order from the eye, the normal is towards the eye.
			normal = v2.subtract(v1).crossProduct(v2.subtract(v3));
			normal.normalize();
		}
		
		void setV(int index, Vector3 value);
		Vector3 checkRayIntersection(Ray ray, Vector3 &normal);		
		void translate(Vector3 translation);
		void rotate_x(double cos_x, double sin_x);
		void rotate_y(double cos_y, double sin_y);
		void rotate_z(double cos_z, double sin_z);
		void scale(Vector3 scaling);

};

