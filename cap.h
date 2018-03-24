#include <vector>
#include "my_utilities.h"
#include "triangle.h"

using namespace std;

// Sphere of radius 1
class Cap : public Object
{
	vector<Triangle> triangles;
	
	public:
		Cap(Vector3 translation,
			Vector3 scaling,
			Vector3 rotation = Vector3(0,0,0),
			Vector3 reflectivity = Vector3(1,1,1),
			double mirrorNess = 0.0,
			double opacity = 1,
			Vector3 weights = Vector3(1,1,1),
			Intensity ambient = Intensity(100,0,0),
			double shininess = 10,
			double refractiveIndex = 0.7
		) : Object(translation, scaling, rotation, reflectivity, mirrorNess, opacity, weights, ambient, shininess, refractiveIndex)
		{
			// Centered at <0,0,0>
			triangles.push_back(Triangle(Vector3(-0.5,0,0.5), Vector3(0.5,0,0.5), Vector3(0,1,0)));
			//~ triangles.push_back(new Triangle(new Vector3(0.5,0,0.5), new Vector3(0.5,0,0.5), new Vector3(0,1,0)));
			//~ triangles.push_back(new Triangle(new Vector3(0.5,0,0.5), new Vector3(-0.5,0,0.5), new Vector3(0,1,0)));
			//~ triangles.push_back(new Triangle(new Vector3(-0.5,0,0.5), new Vector3(-0.5,0,0.5), new Vector3(0,1,0)));
			
			for(int i=0; i<triangles.size(); i++)
			{
				Triangle t = triangles[i];
				t.applyTransformation();
			}
		}
		
		Vector3 checkRayIntersection(Ray ray, Vector3 &normal);
		void translate(Vector3 translation){}
		void rotate_x(double cos_x, double sin_x){}
		void rotate_y(double cos_y, double sin_y){}
		void rotate_z(double cos_z, double sin_z){}
		void scale(Vector3 scaling){}
};


