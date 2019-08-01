#include <assert.h>

#include "group.h"
#include "csg.h"
#include "hit.h"


Group::Group(int objectCount) {
	objectArray.resize(objectCount);
}

Group::~Group() {
	for (Object3D *obj : objectArray) {
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

bool Group::intersectAll(const Ray &r, std::vector<HitPair> &hitArray) {
	for (Object3D *obj : objectArray) {
		std::vector<HitPair> elementHitArray;
		bool hit = obj->intersectAll(r, elementHitArray);
		if (hit)
			hitArray = CSG::computeUnion(hitArray, elementHitArray);
	}
	return !hitArray.empty();
}