#include "transform.h"
#include "ray.h"
#include "hit.h"


Transform::Transform(Matrix &m, Object3D *o): Object3D(nullptr), transform(m), object(o), inverseTransform(m){
	inverseTransform.Inverse();
	inverseTransposeTransform = inverseTransform;
	inverseTransposeTransform.Transpose();
}


bool Transform::intersect(const Ray &r, Hit &h, float tmin) {
	Vec3f o = r.getOrigin();
	inverseTransform.Transform(o);
	Vec3f d = r.getDirection();
	inverseTransform.TransformDirection(d);

	Ray newRay(o, d);
	
	bool isIntersect = object->intersect(newRay, h, tmin);
	if (!isIntersect)
		return false;

	Vec3f normal = h.getNormal();
	inverseTransposeTransform.TransformDirection(normal);
	normal.Normalize();
	h.set(h.getT(), h.getMaterial(), normal, r);
	return true;
}

bool Transform::intersectAll(const Ray &r, std::vector<HitPair> &hitArray) {
	Vec3f o = r.getOrigin();
	inverseTransform.Transform(o);
	Vec3f d = r.getDirection();
	inverseTransform.TransformDirection(d);

	Ray newRay(o, d);

	bool isIntersect = object->intersectAll(newRay, hitArray);
	if (!isIntersect)
		return false;

	for (auto &hitPair : hitArray) {
		Hit &first = hitPair.first;
		Vec3f normal = first.getNormal();
		inverseTransposeTransform.TransformDirection(normal);
		normal.Normalize();
		first = Hit(first.getT(), first.getMaterial(), normal);

		Hit &second = hitPair.second;
		normal = second.getNormal();
		inverseTransposeTransform.TransformDirection(normal);
		normal.Normalize();
		second = Hit(second.getT(), second.getMaterial(), normal);
	}
	return true;
}