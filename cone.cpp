#include "cone.h"

Vector3 Cone :: checkRayIntersection(Ray ray, Vector3 &normal)
{
	Vector3 pos(
		ray.position.x,
		ray.position.y,
		ray.position.z
	);
	Vector3 dir(
		ray.direction.x,
		ray.direction.y,
		ray.direction.z
	);
	removeTransformation();
	pos = removeTransformation(pos, false);
	dir = removeTransformation(dir, true);
	bool discard = false;
	
	// Brought to object coordinate.
	// expressed in parametric form
	// p = pos + dir * t	.. (1)
	// x*x + z*z = (1-y)^2	// Equation of double cone (above/below x-z plane)
	// Solving for t and putting in (1)
	
	double A = dir.x*dir.x + dir.z*dir.z - dir.y*dir.y;
	double B = 2*(pos.x*dir.x + pos.z*dir.z - pos.y*dir.y + dir.y);
	double C = pos.x*pos.x + pos.z*pos.z - (pos.y-1)*(pos.y-1);
	double D = B*B - 4*A*C;
	
	if(D<0) discard = true;
	double t = (-B - sqrt(D)) / (2*A);	// t = (-B + sqrt(D)) / (2*A) gives the other end of the cone;
	
	// If t < 0, the cone is behind the line
	bool isNext = false;
	if(t<0){
		isNext = true;
		t = (-B + sqrt(D)) / (2*A);
		if(t<0) discard = true;
	}
	
	Vector3 p = pos.add(dir.scale(t));

	// p.y<0 refers to the cone below x-z plane
	if(p.y < 0 || p.y > 1)
	{
		if(!isNext)
		{
			t = (-B + sqrt(D)) / (2*A);
			p = pos.add(dir.scale(t));
			isNext = true;
			if(p.y < 0 || p.y > 1) discard = true;
		}
		else discard = true;
	}

	applyTransformation();
	if(discard) return Vector3(0,0,-INF);
	normal.setValues(p.x, sqrt(p.x*p.x+p.z*p.z), p.z);
	p = applyTransformation(p, false);
	normal = applyTransformation(normal, true);
	normal.normalize();

	// Back to world coordinates
	return p;
}
