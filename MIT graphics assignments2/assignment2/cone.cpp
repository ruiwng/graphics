#include "cone.h"
#include "ray.h"
#include "hit.h"

Cone::Cone(float radius, float minY, float maxY, Material *material):Object3D(material), radius(radius), minY(minY), maxY(maxY){}

bool Cone::intersect(const Ray &r, Hit &h, float tmin) {
	// solution of a * x ^ 2 + b * x + c = 0
	Vec3f dir = r.getDirection();
	Vec3f ori = r.getOrigin();

	float height = maxY - minY;
	
	// changed radius
	float x = radius / height;
	float d = radius - x * (ori.y() - minY);
	float e = -x * dir.y();

	float a = dir.x() * dir.x() + dir.z() * dir.z() - e * e;
	float b = 2 * ori.x() * dir.x() + 2 * ori.z() * dir.z() - 2 * d * e;
	float c = ori.x() * ori.x() + ori.z() * ori.z() - d * d;

	float deter = b * b - 4 * a * c;

	if (deter < 0.0f)
		return false;

	float deterSqrt = sqrt(deter);

	// intersection with side face
	float t1 = (-b - deterSqrt) / (2 * a);
	Vec3f inter = r.pointAtParameter(t1);
	if (t1 > tmin && t1 < h.getT() && inter.y() >= minY && inter.y() <= maxY) {
		float alpha = atan(radius / height);
		float beta = atan2(inter.z(), inter.x());
		float cosAlpha = cos(alpha);
		Vec3f normal(cosAlpha * cos(beta), sin(alpha), cosAlpha * sin(beta));
		h.set(t1, material, normal, r);
		return true;
	}

	bool isIntersect = false;
	float t;

	float t2 = (-b + deterSqrt) / (2 * a);
	inter = r.pointAtParameter(t2);
	if (t2 > tmin && t2 < h.getT() && inter.y() >= minY && inter.y() <= maxY) {
		isIntersect = true;
		t = t2;
		float alpha = atan(radius / height);
		float beta = atan2(inter.z(), inter.x());
		float cosAlpha = cos(alpha);
		Vec3f normal(cosAlpha * cos(beta), sin(alpha), cosAlpha * sin(beta));
		h.set(t2, material, normal, r);
	}

	// intersection with bottom circle
	float t3 = (minY - ori.y()) / dir.y();
	inter = r.pointAtParameter(t3);
	float distance2 = inter.x() * inter.x() + inter.z() * inter.z();
	if (distance2 < radius * radius && t3 > tmin && t3 < h.getT()) {
		if (!isIntersect || (t3 < t)) {
			isIntersect = true;
			t = t3;
			h.set(t3, material, Vec3f(0.0f, -1.0f, 0.0f), r);
		}
	}

	return isIntersect;
}