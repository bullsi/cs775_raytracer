#include "my_utilities.h"
#include "ray.h"

#ifndef _Object_H_
#define _Object_H_

using namespace std;

class Object
{
	public:
		// Transformations
		float tx, ty, tz;
		float rx, ry, rz;		// In radians
		float sx, sy, sz;

		double opacity;			// Reflected / (Transmitted + Reflected)
		double refractiveIndex;
		double mirrorNess; 
		
		Vector3 weights;		// Shading weight <amb, diffuse, specular>
		Intensity ambient;
		Vector3 reflectivity;	// Reflected / (Reflected + Absorbed)
		double shininess;
		
		Object(Vector3 translation = Vector3(0,0,0),
			Vector3 scaling = Vector3(1,1,1),
			Vector3 rotation = Vector3(0,0,0),
			Vector3 reflectivity = Vector3(0,0,0),
			double mirrorNess = 1.0,
			double opacity = 1.0,
			Vector3 weights = Vector3(1,1,1),
			Intensity ambient = Intensity(10,0,0),
			double shininess = 10,
			double refractiveIndex = 0.9
		)
		{
			tx = translation.x; ty = translation.y; tz = translation.z;
			sx = scaling.x; sy = scaling.y; sz = scaling.z;
			rx = rotation.x; ry = rotation.y; rz = rotation.z;
			this->reflectivity = reflectivity.clone();
			this->weights = weights.clone();
			this->weights.normalize();
			this->ambient = ambient.clone();
			this->shininess = shininess;
			this->opacity = opacity;
			this->refractiveIndex = refractiveIndex;
			this->mirrorNess = mirrorNess;
		}
		
		// Returns the intersection point of the passed ray with this object
		// Sets the passed normal at the point
		// If the returned value is NULL, then the normal makes no sense
		virtual Vector3 checkRayIntersection(Ray ray, Vector3 &normal){}
		
		void removeTransformation();
		void applyTransformation();
		// Removes/Applies the given transformation from/to the passed vector
		Vector3 removeTransformation(Vector3 v, bool isDirection);
		Vector3 applyTransformation(Vector3 v, bool isDirection);
		
		virtual void translate(Vector3 translation) {}
		virtual void rotate_x(double cos_x, double sin_x) {}
		virtual void rotate_y(double cos_y, double sin_y) {}
		virtual void rotate_z(double cos_z, double sin_z) {}
		virtual void scale(Vector3 scaling) {}
};
#endif
