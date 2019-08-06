#include <algorithm>
#include "cube.h"
#include "ray.h"
#include "hit.h"


Cube::Cube(Material *material) :Object3D(material) {}

bool Cube::intersect(const Ray &r, Hit &h, float tmin) {
	std::vector<HitPair> hitArray;
	bool isHit = intersectAll(r, hitArray);
	if (!isHit)
		return false;

	const Hit &firstHit = hitArray[0].first;
	if (firstHit.getT() >= tmin && firstHit.getT() < h.getT()) {
		h = firstHit;
		return true;
	}

	const Hit &secondHit = hitArray[0].second;
	if (secondHit.getT() >= tmin && secondHit.getT() < h.getT()) {
		h = secondHit;
		return true;
	}
	return false;
}


bool Cube::intersectAll(const Ray &r, std::vector<HitPair> &hitArray) {
	Vec3f ori = r.getOrigin();
	Vec3f dir = r.getDirection();

	float minX = (-1 - ori.x()) / dir.x();
	Vec3f minXNormal(-1.0f, 0.0f, 0.0f);
	float maxX = (1 - ori.x()) / dir.x();
	Vec3f maxXNormal(1.0f, 0.0f, 0.0f);
	if (minX > maxX) {
		std::swap(minX, maxX);
		std::swap(minXNormal, maxXNormal);
	}

	float minY = (-1 - ori.y()) / dir.y();
	Vec3f minYNormal(0.0f, -1.0f, 0.0f);
	float maxY = (1 - ori.y()) / dir.y();
	Vec3f maxYNormal(0.0f, 1.0f, 0.0f);
	if (minY > maxY) {
		std::swap(minY, maxY);
		std::swap(minYNormal, maxYNormal);
	}
		

	float minZ = (-1 - ori.z()) / dir.z();
	Vec3f minZNormal(0.0f, 0.0f, -1.0f);
	float maxZ = (1 - ori.z()) / dir.z();
	Vec3f maxZNormal(0.0f, 0.0f, 1.0f);
	if (minZ > maxZ) {
		std::swap(minZ, maxZ);
		std::swap(minZNormal, maxZNormal);
	}
		
	float minT = std::max(minX, std::max(minY, minZ));
	float maxT = std::min(maxX, std::min(maxY, maxZ));

	if (isnan(minT) || isnan(maxT) || minT > maxT)
		return false;

	Vec3f minNormal, maxNormal;
	if (minT == minX)
		minNormal = minXNormal;
	else if (minT == minY)
		minNormal = minYNormal;
	else
		minNormal = minZNormal;
	
	if (maxT == maxX)
		maxNormal = maxXNormal;
	else if (maxT == maxY)
		maxNormal = maxYNormal;
	else
		maxNormal = maxZNormal;

	Hit minH(minT, material, minNormal);
	Hit maxH(maxT, material, maxNormal);

	hitArray.push_back(make_pair(minH, maxH));
	return true;
}