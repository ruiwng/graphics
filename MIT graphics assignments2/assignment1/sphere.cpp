#include "sphere.h"
#include "ray.h"
#include "hit.h"


Sphere::Sphere(const Vec3f &center, float radius, Material *material):Object3D(material), center(center), radius(radius){}


// algebra method
//bool Sphere::intersect(const Ray &r, Hit &h, float tmin) {
//	Vec3f CtoO = r.getOrigin() - center;
//	Vec3f dir = r.getDirection();
//
//	float a = dir.Dot3(dir);
//	float b = 2 * CtoO.Dot3(dir);
//	float c = CtoO.Dot3(CtoO) - radius * radius;
//	float delta = b * b - 4 * a * c;
//	
//	if (delta < 0.0f)
//		return false;
//	float deltaRoot = sqrt(delta);
//
//	float t1 = (-b - deltaRoot) / (2 * a);
//	bool x1 = t1 >= tmin;
//	float x = h.getT();
//	bool x2 = t1 < h.getT();
//	if (t1 >= tmin && t1 < h.getT()) {
//		h.set(t1, material, r);
//		return true;
//	}
//	
//	float t2 = (-b + deltaRoot) / (2 * a);
//	if (t2 >= tmin && t1 < h.getT()) {
//		h.set(t2, material, r);
//		return true;
//	}
//	return false;
//}

// geometry method
bool Sphere::intersect(const Ray &r, Hit &h, float tmin) {
	Vec3f OtoC = center - r.getOrigin();
	Vec3f dir = r.getDirection();
	float minT = OtoC.Dot3(dir);
	Vec3f minPoint = r.getOrigin() + minT * r.getDirection();
	
	float r2 = radius * radius;
	Vec3f a = minPoint - center;
	float a2 = a.Dot3(a);
	if (a2 > r2)
		return false;
	float b = sqrt(r2 - a2);
	float deltaT = b / dir.Length();
	
	float t1 = minT - deltaT;
	if (t1 >= tmin && t1 < h.getT()) {
		h.set(t1, material, r);
		return true;
	}

	float t2 = minT + deltaT;
	if (t2 >= tmin && t2 < h.getT()) {
		h.set(t2, material, r);
		return true;
	}
	return false;
}