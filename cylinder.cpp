#include "cylinder.h"

Vector3 Cylinder :: checkRayIntersection(Ray ray, Vector3 &normal)
{
	Vector3 pos = Vector3(
		ray.position.x,
		ray.position.y,
		ray.position.z
	);
	Vector3 dir = Vector3(
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
	// x*x + z*z = 1	// Equation of infinite cylinder (above/below x-z plane)
	// Solving for t and putting in (1)
	double A = dir.x*dir.x + dir.z*dir.z;
	double B = 2*(pos.x*dir.x + pos.z*dir.z);
	double C = pos.x*pos.x + pos.z*pos.z - 1;
	double D = B*B - 4*A*C;

	if(D<0) discard = true;
	double t = (-B - sqrt(D)) / (2*A);	// t = (-B + sqrt(D)) / (2*A) gives the other end of the cylinder;
	
	// If t < 0, the cylinder is behind the line
	bool isNext = false;
	if(t<0){
		t = (-B + sqrt(D)) / (2*A);
		isNext = true;
		if(t<0) discard = true;
		// The code will reach here if the eye is inside the cylinder
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
	if(discard){
		p.x = 0;
		p.y = 0;
		p.z = -INF;
		return p;
	}
	
	normal = Vector3(p.x, 0, p.z);
	p = applyTransformation(p, false);
	normal = applyTransformation(normal, true);
	normal.normalize();

	return p;
}
