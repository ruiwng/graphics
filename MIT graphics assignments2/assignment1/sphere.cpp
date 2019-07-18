#include "sphere.h"
#include "ray.h"
#include "hit.h"


Sphere::Sphere(const Vec3f &center, float radius, Material *material):Object3D(material), center(center), radius(radius){}

bool Sphere::intersect(const Ray &r, Hit &h, float tmin) {
	Vec3f CtoO = r.getOrigin() - center;
	Vec3f dir = r.getDirection();

	float a = dir.Dot3(dir);
	float b = 2 * CtoO.Dot3(dir);
	float c = CtoO.Dot3(CtoO) - radius * radius;
	float delta = b * b - 4 * a * c;
	
	if (delta < 0.0f)
		return false;
	float deltaRoot = sqrt(delta);

	float t1 = (-b - deltaRoot) / (2 * a);
	bool x1 = t1 >= tmin;
	float x = h.getT();
	bool x2 = t1 < h.getT();
	if (t1 >= tmin && t1 < h.getT()) {
		h.set(t1, material, r);
		return true;
	}
	
	float t2 = (-b + deltaRoot) / (2 * a);
	if (t2 >= tmin && t1 < h.getT()) {
		h.set(t2, material, r);
		return true;
	}
	return false;
}