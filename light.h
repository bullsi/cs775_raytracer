#include "my_utilities.h"
#include "ray.h"

using namespace std;

class Light
{
	public:
		Vector3 position;
		Intensity color;
		Light(Vector3 position, Intensity color)
		{
			this->position = position;
			this->color = color;
		}
};


