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
			Vec3f normal(inter.x(), 0.0f, inter.z());
			normal.Normalize();
			h.set(t1, material, normal, r);
			return true;
		}

		float t2 = (-b + deterSqrt) / (2 * a);
		inter = r.pointAtParameter(t2);
		if (t2 > tmin && t2 < h.getT() && inter.y() >= minY && inter.y() <= maxY) {
			isIntersect = true;
			t = t2;
			Vec3f normal(inter.x(), 0.0f, inter.z());
			normal.Normalize();
			h.set(t2, material, normal, r);
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
			h.set(t3, material, Vec3f(0.0f, -1.0f, 0.0f), r);
		}
	}

	// intersection with top circle
	float t4 = (maxY - ori.y()) / dir.y();
	inter = r.pointAtParameter(t4);
	distance2 = inter.x() * inter.x() + inter.z() * inter.z();
	if (distance2 < radius * radius && t4 > tmin && t4 < h.getT()) {
		if (!isIntersect || (t4 < t)) {
			isIntersect = true;
			h.set(t4, material, Vec3f(0.0f, 1.0f, 0.0f), r);
		}
	}

	return isIntersect;
}

bool Cylinder::intersectAll(const Ray &r, std::vector<HitPair> &hitArray) {
	Vec3f dir = r.getDirection();
	Vec3f ori = r.getOrigin();

	float a = dir.x() * dir.x() + dir.z() * dir.z();
	float b = 2 * ori.x() * dir.x() + 2 * ori.z() * dir.z();
	float c = ori.x() * ori.x() + ori.z() * ori.z() - radius * radius;

	float deter = b * b - 4 * a * c;

	Vec3f inter;

	std::vector<Hit> hitVec;
	if (deter >= 0.0f) {  // it doesn't mean that ray has not intersection with cylinder when delta is less than 0. 
						  // it occurs when the ray is parallel to the side face of the cylinder and in the interior of it. 
		float deterSqrt = sqrt(deter);

		// intersection with side face
		float t1 = (-b - deterSqrt) / (2 * a);
		inter = r.pointAtParameter(t1);
		if (inter.y() >= minY && inter.y() <= maxY) {
			Vec3f normal(inter.x(), 0.0f, inter.z());
			normal.Normalize();
			hitVec.push_back(Hit(t1, material, normal));
		}

		float t2 = (-b + deterSqrt) / (2 * a);
		inter = r.pointAtParameter(t2);
		if (inter.y() >= minY && inter.y() <= maxY) {
			Vec3f normal(inter.x(), 0.0f, inter.z());
			normal.Normalize();
			hitVec.push_back(Hit(t2, material, normal));
		}
	}

	// intersection with bottom circle
	float t3 = (minY - ori.y()) / dir.y();
	inter = r.pointAtParameter(t3);
	float distance2 = inter.x() * inter.x() + inter.z() * inter.z();
	if (distance2 < radius * radius) {
		hitVec.push_back(Hit(t3, material, Vec3f(0.0f, -1.0f, 0.0f)));
	}

	// intersection with top circle
	float t4 = (maxY - ori.y()) / dir.y();
	inter = r.pointAtParameter(t4);
	distance2 = inter.x() * inter.x() + inter.z() * inter.z();
	if (distance2 < radius * radius) {
		hitVec.push_back(Hit(t4, material, Vec3f(0.0f, 1.0f, 0.0f)));
	}

	if (hitVec.empty())
		return false;

	bool initMin = false, initMax = false;
	Hit minHit, maxHit;

	for (auto &hit : hitVec) {
		if (!initMin || hit.getT() < minHit.getT())
		{
			minHit = hit;
			initMin = true;
		}
		if (!initMax || hit.getT() > maxHit.getT())
		{
			maxHit = hit;
			initMax = true;
		}
	}
	hitArray.push_back(make_pair(minHit, maxHit));

	return true;
}