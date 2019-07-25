#ifndef _PERSPECTIVE_CAMERA_H_
#define _PERSPECTIVE_CAMERA_H_

#include "camera.h"
#include "vectors.h"

class PerspectiveCamera : public Camera {
public:
	PerspectiveCamera() {}
	PerspectiveCamera(Vec3f center, Vec3f direction, Vec3f up, float radian);
	
	virtual Ray generateRay(Vec2f point);
	virtual float getTMin() const {
		return 0.0f;
	}

private:
	Vec3f horizontal;
	Vec3f vertical;
	Vec3f origin;
	Vec3f center;
};

#endif
