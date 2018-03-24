#include "my_utilities.h"

#ifndef _RAY_H_
#define _RAY_H_

using namespace std;

class Ray
{
	public:
		Vector3 position;
		Vector3 direction;
		Ray(Vector3 position, Vector3 direction)
		{
			this->position = position;
			this->direction = direction;
		}
		void translate(Vector3 xyz)
		{
			position = position.subtract(xyz);
		}
};

#endif
