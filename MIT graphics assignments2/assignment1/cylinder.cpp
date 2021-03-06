#include "cylinder.h"
#include "ray.h"
#include "hit.h"


Cylinder::Cylinder(float radius, float minY, float maxY, Material *material):Object3D(material), radius(radius), minY(minY), maxY(maxY){}


bool Cylinder::intersect(const Ray &r, Hit &h, float tmin) {
	// solution of a * x ^ 2 + b * x + c = 0;
	Vec3f dir = r.getDirection();
	Vec3f ori = r.getOrigin();

	float a = dir.x() * dir.x() + dir.z() * dir.z();
	float b = 2 * ori.x() * dir.x() + 2 * ori.z() * dir.z();
	float c = ori.x() * ori.x() + ori.z() * ori.z() - radius * radius;

	float deter = b * b - 4 * a * c;
	
	bool isIntersect = false;
	float t;
	Vec3f inter;

	if (deter >= 0.0f) {  // it doesn't mean that ray has not intersection with cylinder when delta is less than 0. 
		// it occurs when the ray is parallel to the side face of the cylinder and in the interior of it. 
		float deterSqrt = sqrt(deter);

		// intersection with side face
		float t1 = (-b - deterSqrt) / (2 * a);
		inter = r.pointAtParameter(t1);
		if (t1 > tmin && t1 < h.getT() && inter.y() >= minY && inter.y() <= maxY) {
			h.set(t1, material, r);
			return true;
		}

		float t2 = (-b + deterSqrt) / (2 * a);
		inter = r.pointAtParameter(t2);
		if (t2 > tmin && t2 < h.getT() && inter.y() >= minY && inter.y() <= maxY) {
			isIntersect = true;
			t = t2;
		}
	}
	
	// intersection with bottom circle
	float t3 = (minY - ori.y()) / dir.y();
	inter = r.pointAtParameter(t3);
	float distance2 = inter.x() * inter.x() + inter.z() * inter.z();
	if (distance2 < radius * radius && t3 > tmin && t3 < h.getT()) {
		if (!isIntersect || (t3 < t)) {
			isIntersect = true;
			t = t3;
		}
	}

	// intersection with top circle
	float t4 = (maxY - ori.y()) / dir.y();
	inter = r.pointAtParameter(t4);
	distance2 = inter.x() * inter.x() + inter.z() * inter.z();
	if (distance2 < radius * radius && t4 > tmin && t4 < h.getT()) {
		if (!isIntersect || (t4 < t)) {
			isIntersect = true;
			t = t4;
		}
	}

	if (isIntersect)
		h.set(t, material, r);

	return isIntersect;
}