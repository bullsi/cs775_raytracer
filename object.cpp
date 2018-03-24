#include "object.h"

void Object :: removeTransformation()
{
	Vector3 t(-tx, -ty, -tz);
	translate(t);
	rotate_z(cos(-rz), sin(-rz));
	rotate_y(cos(-ry), sin(-ry));
	rotate_x(cos(-rx), sin(-rx));
	Vector3 s(1/sx, 1/sy, 1/sz);
	scale(s);
}
void Object :: applyTransformation()
{
	Vector3 s(sx, sy, sz);
	scale(s);
	rotate_x(cos(rx), sin(rx));
	rotate_y(cos(ry), sin(ry));
	rotate_z(cos(rz), sin(rz));
	Vector3 t(tx, ty, tz);
	translate(t);
}
Vector3 Object :: removeTransformation(Vector3 v, bool isDirection)
{
	if(!isDirection){
		Vector3 t(-tx, -ty, -tz);
		v = v.translate(t);
	}
	v = v.rotate_z(cos(-rz), sin(-rz));
	v = v.rotate_y(cos(-ry), sin(-ry));
	v = v.rotate_x(cos(-rx), sin(-rx));
	Vector3 s(1/sx, 1/sy, 1/sz);
	v = v.scale(s);
	return v;
}
Vector3 Object :: applyTransformation(Vector3 v, bool isDirection)
{
	if(!isDirection){
		Vector3 s(sx, sy, sz);
		v = v.scale(s);
	}
	else{
		Vector3 s(1/sx, 1/sy, 1/sz);
		v = v.scale(s);
	 }
	v = v.rotate_x(cos(rx), sin(rx));
	v = v.rotate_y(cos(ry), sin(ry));
	v = v.rotate_z(cos(rz), sin(rz));
	if(!isDirection){
		Vector3 t(tx, ty, tz);
		v = v.translate(t);
	}
	return v;
}
