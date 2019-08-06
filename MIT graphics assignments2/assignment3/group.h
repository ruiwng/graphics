#ifndef _GROUP_H_
#define _GROUP_H_

#include <vector>

#include "object3D.h"

class Group : public Object3D {
public:
	Group() {}
	Group(int objectCount);

	void addObject(int index, Object3D *obj);
	virtual bool intersect(const Ray &r, Hit &h, float tmin);
	virtual bool intersectAll(const Ray &r, std::vector<HitPair> &hitArray);
	virtual void paint();

private:
	std::vector<Object3D*> objectArray;
};

#endif