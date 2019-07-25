#ifndef _PLANE_H_
#define _PLANE_H_

#include "object3D.h"
#include "vectors.h"
#include "material.h"


class Plane : public Object3D {
public:
	Plane(Vec3f &normal, float d, Material *material);
	virtual bool intersect(const Ray &r, Hit &h, float tmin);

private:
	const Vec3f normal;
	const float d;
};

#endif
