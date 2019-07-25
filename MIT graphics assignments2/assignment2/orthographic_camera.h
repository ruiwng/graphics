#ifndef _ORTHOGRAPHIC_CAMERA_H_
#define _ORTHOGRAPHIC_CAMERA_H_

#include <limits>

#include "camera.h"
#include "vectors.h"

class OrthographicCamera : public Camera {
public:
	OrthographicCamera() {}
	OrthographicCamera(Vec3f center, Vec3f direction, Vec3f up, float size);
	virtual Ray generateRay(Vec2f point);
	virtual float getTMin() const {
		return std::numeric_limits<float>::lowest();
	}
private:
	Vec3f direction;
	Vec3f horizontal;
	Vec3f vertical;
	Vec3f origin;
	float size;
};

#endif
