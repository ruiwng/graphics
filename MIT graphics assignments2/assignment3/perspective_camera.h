#ifndef _PERSPECTIVE_CAMERA_H_
#define _PERSPECTIVE_CAMERA_H_

#include "camera.h"
#include "vectors.h"

class PerspectiveCamera : public Camera {
public:
	PerspectiveCamera() {}
	PerspectiveCamera(Vec3f center, Vec3f direction, Vec3f up, float angle);
	
	virtual Ray generateRay(Vec2f point);
	virtual float getTMin() const {
		return 0.0f;
	}

	virtual void glInit(int w, int h);
	virtual void glPlaceCamera(void);
	virtual void dollyCamera(float dist);
	virtual void truckCamera(float dx, float dy);
	virtual void rotateCamera(float rx, float ry);

	virtual void initRayTracing();
private:
	Vec3f horizontalDir;
	Vec3f verticalDir;
	Vec3f origin;
	Vec3f center;

	Vec3f direction;
	Vec3f up;
	Vec3f radian;
	float angle;
};

#endif
