#ifndef _SPHERE_H_
#define _SPHERE_H_

#include "object3D.h"
#include "vectors.h"
#include "material.h"


class Sphere: public Object3D {
public:
	Sphere(const Vec3f &center, float radius, Material* material);
	virtual bool intersect(const Ray &r, Hit &h, float tmin);
	virtual bool intersectAll(const Ray &r, std::vector<HitPair> &hitArray);

	virtual void paint();
private:
	const Vec3f center;
	const float radius;

	bool isPaintInit;
	std::vector<std::vector<Vec3f>> pointArray;
	std::vector<std::vector<Vec3f>> normalArray;
};

#endif
