#include "cap.h"

//~ Vector3 Cap :: checkRayIntersection(Ray ray, Vector3 &normal)
//~ {
	//~ Vector3 pos = new Vector3(
		//~ ray.position.x,
		//~ ray.position.y,
		//~ ray.position.z
	//~ );
	//~ Vector3 dir = new Vector3(
		//~ ray.direction.x,
		//~ ray.direction.y,
		//~ ray.direction.z
	//~ );
//~ 
	//~ removeTransformation();
	//~ pos.show();
	//~ dir.show();
	//~ pos = removeTransformation(pos, false);
	//~ dir = removeTransformation(dir, true);
	//~ cout << "A ";pos.show();
	//~ dir.show();
	//~ 
	//~ Vector3 nearestPoint = Vector3(0,0,-INF);
	//~ Vector3 nearestNormal = Vector3(0,0,-INF);
	//~ for(int i=0; i<triangles.size(); i++)
	//~ {
		//~ Triangle t = triangles[i];
		//~ Vector3 normal;
		//~ Vector3 v = triangles[i].checkRayIntersection(new Ray(pos,dir), normal);
		//~ if(v != Vector3(0,0,-INF))
		//~ {
			//~ if (nearestPoint == Vector3(0,0,-INF) || v.magnitude_sq() < nearestPoint.magnitude_sq())
			//~ {
				//~ nearestPoint = v;
				//~ nearestNormal = normal;
			//~ }
		//~ }
	//~ }
	//~ applyTransformation();
	//~ if(nearestPoint != Vector3(0,0,-INF))
	//~ {
		//~ nearestPoint = applyTransformation(nearestPoint, false);
		//~ normal = applyTransformation(normal, true);
		//~ normal.normalize();
	//~ }
	//~ return nearestPoint;
//~ }

Vector3 Cap :: checkRayIntersection(Ray ray, Vector3 &normal)
{
	Vector3 nearestV = Vector3(0,0,-INF);
	Vector3 nearestN;
	for(int i=0; i<triangles.size(); i++)
	{
		Vector3 n;
		Vector3 v = triangles[i].checkRayIntersection(ray, n);
		if(nearestV.isNull() || v.magnitude_sq() < nearestV.magnitude_sq())
		{
			nearestV = v;
			nearestN = n;
		}
	}
	if(!nearestV.isNull()) normal = Vector3(nearestN.x, nearestN.y, nearestN.z);
	return nearestV;
}
