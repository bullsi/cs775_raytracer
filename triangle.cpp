#include "triangle.h"

void Triangle :: setV(int index, Vector3 value)
{
	v[index] = value;
	normal = v[1].subtract(v[0]).crossProduct(v[1].subtract(v[2]));
	normal.normalize();
}

void Triangle :: translate(Vector3 translation)
{ for(int i=0; i<3; i++) setV(i, v[i].add(translation)); }
void Triangle :: rotate_x(double cos_x, double sin_x)
{ for(int i=0; i<3; i++) setV(i, v[i].rotate_x(cos_x, sin_x)); }
void Triangle :: rotate_y(double cos_y, double sin_y)
{ for(int i=0; i<3; i++) setV(i, v[i].rotate_y(cos_y, sin_y)); }
void Triangle :: rotate_z(double cos_z, double sin_z)
{ for(int i=0; i<3; i++) setV(i, v[i].rotate_z(cos_z, sin_z)); }
void Triangle :: scale(Vector3 scaling)
{ for(int i=0; i<3; i++) setV(i, v[i].scale(scaling)); }

double min(double a, double b, double c)
{ return (a<c? (a<b?a:b): (c<b?c:b)); }
double max(double a, double b, double c)
{ return (a<c? (c<b?b:c): (a<b?a:b)); }

Vector3 Triangle :: checkRayIntersection(Ray ray, Vector3 &normal)
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
	bool discard = false;
	Vector3 n = this->normal;
	Vector3 translation = Vector3(v[2].x, v[2].y, v[2].z);
	
	// Rotate the triangle around v3 to align its normal to -z axis
	// Rotate around v3
	double cos_y = -n.z / sqrt(n.x*n.x + n.z*n.z);
	double sin_y = n.x / sqrt(n.x*n.x + n.z*n.z);
	double cos_x = sqrt(1-n.y*n.y) / 1;
	double sin_x = n.y / 1;
	
	pos = pos.translate(translation.negate());
	pos = pos.rotate_y(cos_y, sin_y);
	pos = pos.rotate_x(cos_x, sin_x);
	dir = dir.rotate_y(cos_y, sin_y);
	dir = dir.rotate_x(cos_x, sin_x);
	
	// Now the triangle is at z=0 plane
	// The ray touches z=0 at
	//~ 
	//~ cout << "ray "; pos.show(); dir . show();
	//~ cout << "v1 "; v[0].show();
	//~ cout << "v2 "; v[1].show();
	//~ cout << "v3 "; v[2].show();
	//~ 
	double t;
	if(dir.z != 0) t = -pos.z / dir.z;
	else return Vector3(0,0,-INF);
	if(t<0) return Vector3(0,0,-INF);
	
	translate(translation.negate());
	rotate_y(cos_y, sin_y);
	rotate_x(cos_x, sin_x);
	
	double x = pos.x + dir.x*t;
	double y = pos.y + dir.y*t;
	
	//~ cout << "t = " << t << " " << x << " " << y << endl;
	if(x <= min(v[0].x,v[1].x,v[2].x)) discard = true;
	else if(x >= max(v[0].x,v[1].x,v[2].x)) discard = true;
	
	if( (v[2].x-v[1].x>0 && v[1].x<x && x<v[2].x)
	 || (v[2].x-v[1].x<0 && v[2].x<x && x<v[1].x)
	)
	{
		//~ cout << "A" << endl;
		double cal_y = v[1].y + (v[1].y - v[2].y)/(v[1].x - v[2].x) * (x - v[1].x);
		double loc_y = v[1].y + (v[1].y - v[2].y)/(v[1].x - v[2].x) * (v[0].x - v[1].x);
		// If v[0] lies below the v[1]-v[2], then discard
		if((y - cal_y)/(v[0].y - loc_y) < 0) discard = true;
	}
	if( (v[0].x-v[2].x>0 && v[2].x<x && x<v[0].x)
	 || (v[0].x-v[2].x<0 && v[0].x<x && x<v[2].x)
	 )
	{
		//~ cout << "A1" << endl;
		double cal_y = v[2].y + (v[2].y - v[0].y)/(v[2].x - v[0].x) * (x - v[2].x);
		double loc_y = v[2].y + (v[2].y - v[0].y)/(v[2].x - v[0].x) * (v[1].x - v[2].x);
		// If v[1] lies below the v[0]-v[2], then discard
		if((y - cal_y)/(v[1].y - loc_y) < 0) discard = true;
	}
	if( (v[1].x-v[0].x>0 && v[0].x<x && x<v[1].x)
	 || (v[1].x-v[0].x<0 && v[1].x<x && x<v[0].x)
	 )
	{
		//~ cout << "C" << endl;
		double cal_y = v[0].y + (v[0].y - v[1].y)/(v[0].x - v[1].x) * (x - v[0].x);
		double loc_y = v[0].y + (v[0].y - v[1].y)/(v[0].x - v[1].x) * (v[2].x - v[0].x);
		//~ cout << y << "  " << cal_y << "  " << v[2].y << " " << loc_y << endl;
		// If v[2] lies below the v[0]-v[1], then discard
		if((y - cal_y)/(v[2].y - loc_y) < 0) discard = true;
	}
	
	rotate_x(cos_x, -sin_x);
	rotate_y(cos_y, -sin_y);
	translate(translation);
	//~ cout << discard << " <"<<x<<","<<y<<",0>"<<endl; 
	if(discard) return Vector3(0,0,-INF);
	
	Vector3 p = Vector3(x,y,0);																																																																																																							
	// Do the same reverse transformation on p too
	p = p.rotate_x(cos_x, -sin_x);
	p = p.rotate_y(cos_y, -sin_y);
	p = p.translate(translation);
	
	normal = Vector3(n.x, n.y, n.z);
	
	return p;
}
