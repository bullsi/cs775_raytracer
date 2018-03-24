//** How to merge object color with the light's color

#include <iostream>
#include <vector>

#include "light.h"
#include "camera.h"

#include "cap.h"
#include "cylinder.h"
#include "cone.h"
#include "sphere.h"

#include "ray.h"
#include "my_utilities.h"

using namespace std; 

int X,Y;
int width, height;
vector<vector <Intensity> > screen;

vector<Object*> objects;
vector<Light> lights;
Camera camera;																															

void loadScene()
{
	cin >> width >> height;
	double z;
	cin >> z;
	camera.origin = Vector3(0,0,z);
	cin >> camera.nearPlane >> camera.farPlane >> camera.left >> camera.right >> camera.bottom >> camera.top;
	int n;
	cin >> n;
	for(int i=0; i<n; i++)
	{
		double x,y,z;
		cin >> x >> y >>z;
		Vector3 pos(x,y,z);
		cin >> x >> y >>z;
		Intensity in(x,y,z);
		Light light(pos,in);
		lights.push_back(light);
	}
	cin >> n;
	for(int i=0; i<n; i++)
	{
		int type;
		cin >> type;
		double x,y,z;
		cin >> x >> y >> z;
		Vector3 a(x,y,z);
		cin >> x >> y >> z;
		Vector3 b(x,y,z);
		cin >> x >> y >> z;
		Vector3 c(x,y,z);
		cin >> x >> y >> z;
		Vector3 d(x,y,z);
		double mi, op;
		cin >> mi >> op;
		
		if(type==1)
		{
			Sphere* sp = new Sphere(a,b,c,d,mi,op);
			objects.push_back(sp);
		}
		else if(type==2)
		{
			Cone* co = new Cone(a,b,c,d,mi,op);
			objects.push_back(co);
		}
		else if(type==3)
		{
			Cylinder* cy = new Cylinder(a,b,c,d,mi,op);
			objects.push_back(cy);
		}
		else if(type==4)
		{
			Triangle* tr = new Triangle(a,b,c,d,mi,op);
			objects.push_back(tr);
		}
	}
}

// Check if there is a direct path from v1 to v2
bool checkDirectPath(Vector3 v1, Vector3 v2)
{
	Vector3 dir = v2.subtract(v1);
	Ray ray(v1, dir);

	for(int i=0; i<objects.size(); i++)
	{
		Vector3 normal;
		Vector3 v = objects[i]->checkRayIntersection(ray, normal);
		//~ v.show();
		// If the ray intersects some object
		if(!v.isNull() && !v.equals(v1)){
			// Its refractive, continue.. ** Not a good method
			if(objects[i]->opacity < 0.5) continue;
			return false;
		}
	}
	return true;
}

Intensity traceRay(Ray ray, int level)
{
	//~ cerr << level << " ";
	if(level <= 0){
		Intensity temp(0,0,0);
		return temp;
	}

	//~ cerr << "POP{" << objects[0]->sx << " " << objects[0]->sy << " " << objects[0]->sz << endl;
	Vector3 nearestPointN;
	Vector3 nearestPoint(0,0,-INF);
	Object* nearestObject;
	// Get the nearest point of intersection
	for(int i=0; i<objects.size(); i++)
	{
		Vector3 normal;
		Object* o = objects[i];
		Vector3 v = o->checkRayIntersection(ray, normal);
		// v is not null, then the ray intersects some object
		if(!v.isNull())
		{
			if (nearestPoint.isNull() || v.magnitude_sq() < nearestPoint.magnitude_sq())
			{
				nearestPoint = v;
				nearestPointN = normal;
				nearestObject = o;
			}
		}
	}
	
	Intensity intensity, intensityRefracted, intensityReflected, intensityMirrored;
	//~ // If the ray intersects some object
	if(!nearestPoint.isNull())
	{
		double cos_EN = ray.direction.cos(nearestPointN);	// Between E and N
		// Do lighting calculations at the nearest point found above
		for(int j=0; j<lights.size(); j++)
		{
			// Check if the light is visible from this point
			if(!checkDirectPath(nearestPoint, lights[j].position))
			{
				continue;
			}
			Vector3 lightV = nearestPoint.subtract(lights[j].position);
			// R = L - (a)*N; a = 2*L.N
			double a = 2*lightV.dotProduct(nearestPointN);
			Vector3 lightRefl = lightV.subtract( nearestPointN.scale(a) );
			//~ cerr << "refl "; lightRefl.show();
			
			double cos_LN = lightV.cos(nearestPointN);				// Between L and N
			double cos_ER = ray.direction.cos(lightRefl);			// Between E and R
			
			if(cos_EN * cos_LN < 0) continue;						// Light is on the opposite side of the surface (won't account for usual lighting)
			
			Intensity amb = nearestObject->ambient;
			Intensity diff = lights[j].color.scale( -cos_LN );
			Intensity spec = lights[j].color.scale( pow(-cos_ER, nearestObject->shininess) );
			
			intensityReflected = intensityReflected.add(amb.scale(nearestObject->weights.x));
			intensityReflected = intensityReflected.add(diff.scale(nearestObject->weights.y));
			intensityReflected = intensityReflected.add(spec.scale(nearestObject->weights.z));
			
			intensityReflected = intensityReflected.scale(nearestObject->reflectivity);
		}
		// intensityReflected contains the opaque intensity
		
		// Refraction
		// T = (b)*N + (nL/nT)*E; b = (nL/nT)*E.N - sqrt[1 - (nL/nT)(1-(E.N)^2)]
		double nL_nT = 1 / nearestObject->refractiveIndex;
		double EN = cos_EN;
		if(cos_EN>0)					// Approaching the surface from the back
		{
			nL_nT = 1/nL_nT;
			EN *= -1;
		}
		double b = nL_nT*-EN - sqrt(1 - nL_nT*nL_nT*(1-EN*EN));
		Vector3 rayRefr = nearestPointN.scale(b) . add( ray.direction.scale(nL_nT) );
		rayRefr.normalize();
		if(nearestObject->opacity != 1)
		{
			Ray r(nearestPoint.add(rayRefr.scale(0.1)), rayRefr);
			intensityRefracted = traceRay(r, level-1);
		}
		
		// Reflection
		// R = E - (a)*N; a = 2*E.N
		double a = 2*ray.direction.dotProduct(nearestPointN);
		Vector3 rayRefl = ray.direction.subtract( nearestPointN.scale(a) );
		rayRefl.normalize();
		
		if(nearestObject->mirrorNess != 0)
		{
			Ray r(nearestPoint.add(rayRefl.scale(0.1)), rayRefl);
			intensityMirrored = traceRay(r, level-1);
		}
		
		//Net intensity = (1-opacity)<Refracted> + (opacity)((1-mirrorNess)<Usual or Relfected> + mirrorNess<Mirror>
		intensity =	
			intensityRefracted . scale (1-nearestObject->opacity) . add (
				intensityReflected . scale(1-nearestObject->mirrorNess) . add(intensityMirrored.scale(nearestObject->mirrorNess)) . scale(nearestObject->opacity)
			);
	}
	if(intensity.R < 0) intensity.R=0;
	else if(intensity.R > 255) intensity.R=255;
	if(intensity.G < 0) intensity.G=0;
	else if(intensity.G > 255) intensity.G=255;
	if(intensity.B < 0) intensity.B=0;
	else if(intensity.B > 255) intensity.B=255;
	
	//~ if(nearestPoint==Vector3(0,0,-INF)){
		//~ if(!intensity)cout << "** ";
	//~ }
	//~ intensity.show();
	
	return intensity;
}

int main(int argc, char *argv[])
{
	loadScene();
	
	for(int i=0; i<width; i++)
	{
		vector<Intensity> col;
		for(int j=0; j<height; j++)
		{
			col.push_back(Intensity());
		}
		screen.push_back(col);
	}
	
	double xInt = (camera.right - camera.left)/width;
	double yInt = (camera.bottom - camera.top)/height;
	for(int i=0; i<width; i++)
	{
		for(int j=0; j<height; j++)
		{
			double x = (i - width/2)*xInt;
			double y = (j - height/2)*yInt;
			double z = camera.nearPlane;
			
			//~ if(y==0.2)
			{
				int count = 0;
				Intensity sum;
				for(double xx = x-xInt/2; xx<= x+xInt/2; xx+=xInt/2)
				{
					for(double yy = y+yInt/2; yy<= y-yInt/2; yy-=yInt/2)
					{
						Vector3 pos(xx,yy,z);
						Vector3 dir(xx,yy,z);
						Ray ray(pos, dir);
						dir.normalize();
						sum = sum.add( traceRay(ray,3) );
						count++;
					}
				}
				screen[i][j] = sum.scale(1.0/count);

				//~ cerr << i << " " << j << endl;
				//~ X = i; Y = j;
				//~ Vector3 pos(x,y,z);
				//~ Vector3 dir = pos.subtract(camera.origin);
				//~ Ray ray(pos, dir);
				//~ if(i==220	 && j==256)
				//~ {
					//~ screen[i][j] = traceRay(ray, 3);
				//~ }
				
				//~ cout << "I "; screen[i][j].show();
				//~ pos.show();
				//~ sum.show();
				//~ screen[i][j].show();
			}
		}
	}
	// Writing
	cout << "P3" << endl;
	cout << width << " " << height << endl;
	cout << (int)maxIntensity << endl;
	
	for(int j=0; j<height; j++)
	{
		for(int i=0; i<width; i++)
		{
			cout << (unsigned short)screen[i][j].R << " " << (unsigned short)screen[i][j].G << " " << (unsigned short)screen[i][j].B << " ";
		}
		cout << endl;
	}
	return 0;
}
