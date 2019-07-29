#ifndef _CUBE_H_
#define _CUBE_H_


#include "object3D.h"
#include "material.h"


class Cube : public Object3D {
public:
	Cube(Material *material);
	virtual bool intersect(const Ray &r, Hit &h, float tmin);
	virtual bool intersectAll(const Ray &r, std::vector<HitPair> &hitArray);
};
#endif