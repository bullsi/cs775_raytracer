#include "sphere.h"

Vector3 Sphere :: checkRayIntersection(Ray ray, Vector3 &normal)
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
	// Brought to object coordinate. Now sphere is of unit radius	
	// expressed in parametric form
	// p = pos + dir * t	.. (1)
	// x*x + y*y + z*z = 1
	// Solving for t and putting in (1)
	double dd = dir.magnitude_sq();
	double dp = dir.dotProduct(pos);
	double pp = pos.magnitude_sq();
	double D = dp*dp - dd*(pp-1);
	// If D < 0, no solution exists. The line does not touches the sphere.
	if(D<0) discard = true;
	
	double t = (-dp - sqrt(D)) / dd;	// t = (-dp + sqrt(...).. gives the other end of the sphere;
	
	// If t < 0, the sphere is behind the line
	bool isNext = false;
	if(t<0){
		isNext = true;
		t = (-dp + sqrt(D)) / dd;
		if(t<0){
			discard = true;
		}
	}
	applyTransformation();
	if(discard) return Vector3(0,0,-INF);
	Vector3 p = pos.add(dir.scale(t));
	// Normal is <p> - <center (0,0,0)>
	normal = Vector3(p.x, p.y, p.z);
	p = applyTransformation(p, false);
	normal = applyTransformation(normal, true);
	normal.normalize();

	// Back to world coordinates
	return p;
}

