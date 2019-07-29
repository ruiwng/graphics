#ifndef _OBJECT3D_H_
#define _OBJECT3D_H_

#include <vector>

class Ray;
class Hit;
class Material;

typedef std::pair<Hit, Hit> HitPair;

class Object3D {
public:
	Object3D() { material = nullptr; }
	virtual ~Object3D() {}
	Object3D(Material *material) :material(material) {}
	virtual bool intersect(const Ray &t, Hit &h, float tmin) = 0;
	virtual bool intersectAll(const Ray &t, std::vector<HitPair> &hitArray) { return false; };
protected:
	Material *material;
};

#endif
