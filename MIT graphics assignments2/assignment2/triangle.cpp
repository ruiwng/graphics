#include "triangle.h"
#include "ray.h"
#include "hit.h"
#include "matrix.h"


Triangle::Triangle(const Vec3f & a, const Vec3f & b, const Vec3f & c, Material * material): a(a), b(b), c(c), material(material){
	Vec3f::Cross3(normal, b - a, c - b);
	normal.Normalize();
}


bool Triangle::intersect(const Ray &r, Hit &h, float tmin) {
	Vec3f ab = b - a;
	Vec3f ac = c - a;
	Vec3f d = r.getDirection();
	Vec3f o = r.getOrigin();
	Vec3f ao = o - a;

	float deter = det3x3(ab.x(), ac.x(), -d.x(),
		ab.y(), ac.y(), -d.y(), 
		ab.z(), ac.z(), -d.z());

	float deter2 = det3x3(ao.x(), ac.x(), -d.x(),
		ao.y(), ac.y(), -d.y(),
		ao.z(), ac.z(), -d.z());

	float alpha = deter2 / deter;
	if (alpha < 0.0f || alpha > 1.0f)
		return false;

	float deter3 = det3x3(ab.x(), ao.x(), -d.x(),
		ab.y(), ao.y(), -d.y(),
		ab.z(), ao.z(), -d.z());
	float beta = deter3 / deter;
	if (beta < 0.0f || alpha + beta > 1.0f)
		return false;

	float deter4 = det3x3(ab.x(), ac.x(), ao.x(),
		ab.y(), ac.y(), ao.y(),
		ab.z(), ac.z(), ao.z());
	float t = deter4 / deter;
	if (t >= tmin && t < h.getT()) {
		h.set(t, material, normal, r);
		return true;
	}
	return false;
}
