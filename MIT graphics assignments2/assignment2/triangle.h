#ifndef _TRIANGLE_H_
#define _TRIANGLE_H_

#include "object3D.h"
#include "vectors.h"
#include "material.h"


class Triangle : public Object3D {
public:
	Triangle(const Vec3f &a, const Vec3f &b, const Vec3f &c, Material *material);
	virtual bool intersect(const Ray &r, Hit &h, float tmin);

private:
	Vec3f a, b, c;
	Material *material;
	Vec3f normal;
};

#endif
