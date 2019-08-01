#include <assert.h>

#include "group.h"

Group::Group(int objectCount) {
	objectArray.resize(objectCount);
}

Group::~Group() {
	for (Object3D* obj : objectArray) {
		delete obj;
	}
	objectArray.clear();
}

void Group::addObject(int index, Object3D *obj) {
	assert(index >= 0 && index < objectArray.size());
	objectArray[index] = obj;
}

bool Group::intersect(const Ray &r, Hit &h, float tmin) {
	bool isHit = false;
	for (Object3D *obj : objectArray) {
		bool hit = obj->intersect(r, h, tmin);
		if (hit)
			isHit = true;
	}
	return isHit;
}