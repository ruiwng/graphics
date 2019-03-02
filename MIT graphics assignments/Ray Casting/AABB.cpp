#include "AABB.h"

AABB::AABB(const Vector3f& min, const Vector3f& max) :min(min), max(max) {}

void AABB::extend(const Vector3f& p) {
	for (int i = 0; i < 3; ++i)
	{
		if (p[i] < min[i])
			min[i] = p[i];
		if (p[i] > max[i])
			max[i] = p[i];
	}
}

bool AABB::intersect(const Ray& r, Hit& h, float tmin) {
	float t1 = FLT_MIN, t2 = FLT_MAX;
	Vector3f o = r.getOrigin();
	Vector3f d = r.getDirection();
	for (int i = 0; i < 3; ++i) {
		float x1 = (min[i] - o[i]) / d[i];
		float x2 = (max[i] - o[i]) / d[i];
		if (x1 > x2)
			swap(x1, x2);
		if (x1 > t1)
			t1 = x1;
		if (x2 < t2)
			t2 = x2;
		if (t1 > t2)
			return false;
	}
	if (t2 < tmin || t1 > h.getT())
		return false;
	return true;
}