#ifndef _CAMERA_H_
#define _CAMERA_H_

class Ray;
class Vec2f;
class Vec3f;

class Camera {
public:
	virtual Ray generateRay(Vec2f point) = 0;
	virtual float getTMin() const = 0;
};

#endif
