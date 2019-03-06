#ifndef CAMERA_H
#define CAMERA_H

#include "Ray.h"
#include <vecmath.h>
#include <float.h>
#include <cmath>


class Camera
{
public:
	//generate rays for each screen-space coordinate
	virtual Ray generateRay( const Vector2f& point ) = 0 ; 
	
	virtual float getTMin() const = 0 ; 
	virtual ~Camera(){}
};

///TODO: Implement Perspective camera
///Fill in functions and add more fields if necessary
class PerspectiveCamera: public Camera
{
public:
	PerspectiveCamera(const Vector3f& center, const Vector3f& direction, const Vector3f& up, float angle);

	virtual Ray generateRay(const Vector2f& point);

	virtual float getTMin() const { 
		return 0.0f;
	}

private:
	Vector3f u, v, w;
	float angle;
	Vector3f center;
	Vector3f lowerLeftCorner;
	float verticalLength;
	float horizontalLength;
};

#endif //CAMERA_H
