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

	virtual void glInit(int w, int h);
	virtual void glPlaceCamera(void);
	virtual void dollyCamera(float dist);
	virtual void truckCamera(float dx, float dy);
	virtual void rotateCamera(float rx, float ry);

	virtual void initRayTracing();
private:
	Vec3f direction;
	Vec3f horizontalDir;
	Vec3f verticalDir;
	Vec3f origin;

	Vec3f center;
	Vec3f up;
	float size;
};

#endif
