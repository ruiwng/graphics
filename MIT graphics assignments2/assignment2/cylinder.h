#ifndef _CYLINDER_H_
#define _CYLINDER_H_

#include "object3D.h"
#include "vectors.h"
#include "material.h"


class Cylinder : public Object3D {
public:
	Cylinder(float radius, float minY, float maxY, Material *material);
	virtual bool intersect(const Ray &r, Hit &h, float tmin);

private:
	float radius;
	float minY;
	float maxY;
};

#endif