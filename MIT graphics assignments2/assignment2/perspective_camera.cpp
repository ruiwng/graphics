#include "perspective_camera.h"
#include "ray.h"
#include "vectors.h"

#include <math.h>

PerspectiveCamera::PerspectiveCamera(Vec3f center, Vec3f direction, Vec3f up, float radian) :center(center){
	direction.Normalize();
	Vec3f z = direction;
	z.Negate();
	Vec3f::Cross3(this->horizontal, up, z);
	this->horizontal.Normalize();
	Vec3f::Cross3(this->vertical, z, this->horizontal);

	Vec3f screenCenter = center - 0.5f / tan(radian * 0.5f) * z;
	origin = screenCenter - this->horizontal * 0.5f - this->vertical * 0.5f;
}

Ray PerspectiveCamera::generateRay(Vec2f point) {
	Vec3f screenPoint = origin + this->horizontal * point.x() + this->vertical * point.y();
	Vec3f dir = screenPoint - center;
	dir.Normalize();
	return Ray(center, dir);
}
