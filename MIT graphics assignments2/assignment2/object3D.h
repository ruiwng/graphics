#ifndef _OBJECT3D_H_
#define _OBJECT3D_H_

class Ray;
class Hit;
class Material;

class Object3D {
public:
	Object3D() { material = nullptr; }
	virtual ~Object3D() {}
	Object3D(Material *material) :material(material) {}
	virtual bool intersect(const Ray &t, Hit &h, float tmin) = 0;

protected:
	Material *material;
};

#endif
