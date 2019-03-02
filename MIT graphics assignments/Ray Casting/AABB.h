#ifndef  AABB_H
#define  AABB_H

#include "Object3D.h"
#include "Vector3f.h"

class AABB : public Object3D {
public:
	AABB(const Vector3f& min=Vector3f(FLT_MAX, FLT_MAX, FLT_MAX), const Vector3f& max=Vector3f(FLT_MIN, FLT_MIN, FLT_MIN));

	~AABB() {}
	void extend(const Vector3f& p);
	virtual bool intersect(const Ray& r, Hit& h, float min);

private:
	Vector3f min, max;
};

#endif