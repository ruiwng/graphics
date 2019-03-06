#include "Transform.h"

Transform::Transform(const Matrix4f& m, Object3D* obj):m(m), o(obj){}

bool Transform::intersect(const Ray& r, Hit& h, float tmin) {
	Matrix4f transform = m.inverse();
	Vector4f x = transform * Vector4f(r.getOrigin(), 1.0f);
	Vector3f oOrigin = (transform * Vector4f(r.getOrigin(), 1.0f)).xyz();
	Vector3f oDir = (transform * Vector4f(r.getDirection(), 0.0f)).xyz();
	Ray oRay(oOrigin, oDir);
	bool isHit = o->intersect(oRay, h, tmin);
	if (isHit) {
		Vector4f wNormal = transform.transposed() * Vector4f(h.getNormal(), 0.0f);
		h.set(h.getT(), h.getMaterial(), wNormal.xyz().normalized());
	}
	return isHit;
}