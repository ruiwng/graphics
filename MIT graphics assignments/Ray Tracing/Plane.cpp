#include "Plane.h"

Plane::Plane(const Vector3f& normal, float d, Material* m) :Object3D(m), normal(normal), d(d) {
}

bool Plane::intersect(const Ray& r, Hit& h, float tmin) {
	float dn = Vector3f::dot(r.getDirection(), normal);
	if (abs(dn) < 0.0001f)
		return false;
	float on = Vector3f::dot(r.getOrigin(), normal);
	
	float t = (d - on) / dn;
	if (t > tmin && t < h.getT()) {
		h.set(t, material, normal);
		return true;
	}
	return false;
}