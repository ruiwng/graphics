#include "plane.h"
#include "ray.h"
#include "hit.h"


Plane::Plane(Vec3f &normal, float d, Material *material): Object3D(material), normal(normal), d(d){
	normal.Normalize();
}

bool Plane::intersect(const Ray &r, Hit &h, float tmin) {
	Vec3f orig = r.getOrigin();
	Vec3f dir = r.getDirection();

	float t = (d - orig.Dot3(normal)) / (dir.Dot3(normal));
	if (t >= tmin && t < h.getT()) {
		h.set(t, material, normal, r);
		return true;
	}
	return false;
}
