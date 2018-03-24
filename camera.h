#include "my_utilities.h"
#include "ray.h"

using namespace std;

class Camera
{
	public:
		// Considering the eye at the origin
		Vector3 origin;
		double nearPlane;
		double farPlane;
		double left;
		double right;
		double bottom;
		double top;
		Camera()
		{
			nearPlane = 1;
			farPlane = 1000;
			left = -0.5;
			right = 0.5;
			bottom = -0.5;
			top = 0.5;
		}
};


