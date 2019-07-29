#ifndef _TRANSFORM_H_
#define _TRANSFORM_H_

#include "object3D.h"
#include "matrix.h"


class Transform : public Object3D {
public:
	Transform(Matrix &m, Object3D *o);
	virtual bool intersect(const Ray &r, Hit &h, float tmin);
	virtual bool intersectAll(const Ray &r, std::vector<HitPair> &hitArray);

private:
	Matrix transform;
	Matrix inverseTransform;
	Matrix inverseTransposeTransform;
	Object3D *object;
};

#endif