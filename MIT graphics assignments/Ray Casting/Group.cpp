#include <assert.h>

#include "Group.h"

Group::Group(int num_objects):objectCount(num_objects), objectArray(vector<Object3D*>(num_objects)){
}

void Group::addObject(int index, Object3D* obj) {
	assert(index >= 0 && index < objectCount);
	objectArray[index] = obj;
}

bool Group::intersect(const Ray& r, Hit& h, float tmin) {
	bool isHit = false;
	for (auto obj : objectArray) {
		if (obj->intersect(r, h, tmin)) {
			isHit = true;
		}
	}
	return isHit;
}

