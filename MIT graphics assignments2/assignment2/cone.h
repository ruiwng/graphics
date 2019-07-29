#ifndef _CONE_H_
#define _CONE_H_

#include "object3D.h"
#include "vectors.h"
#include "material.h"

class Cone : public Object3D {
public:
	Cone(float radius, float minY, float maxY, Material *material);
	virtual bool intersect(const Ray &r, Hit &h, float tmin);
	virtual bool intersectAll(const Ray &r, std::vector<HitPair> &hitArray);

private:
	float radius;
	float minY;
	float maxY;
};

#endif
