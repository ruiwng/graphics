#ifndef _CAMERA_H_
#define _CAMERA_H_

class Ray;
class Vec2f;
class Vec3f;

class Camera {
public:
	virtual Ray generateRay(Vec2f point) = 0;
	virtual float getTMin() const = 0;

	virtual void glInit(int w, int h) = 0;
	virtual void glPlaceCamera(void) = 0;
	virtual void dollyCamera(float dist) = 0;
	virtual void truckCamera(float dx, float dy) = 0;
	virtual void rotateCamera(float rx, float ry) = 0;

	virtual void initRayTracing() = 0;
};

#endif
