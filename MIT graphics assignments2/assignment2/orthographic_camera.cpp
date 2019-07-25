#include "orthographic_camera.h"
#include "ray.h"
#include "vectors.h"

OrthographicCamera::OrthographicCamera(Vec3f center, Vec3f direction, Vec3f up, float size) {
	this->direction = direction;
	this->direction.Normalize();
	Vec3f z = this->direction;
	z.Negate();
	Vec3f::Cross3(this->horizontal, up, z);
	this->horizontal.Normalize();
	Vec3f::Cross3(this->vertical, z, this->horizontal);
	this->size = size;
	origin = center - this->horizontal * this->size * 0.5f - this->vertical * this->size * 0.5f;
}

Ray OrthographicCamera::generateRay(Vec2f point) {
	Vec3f o = origin + this->horizontal * point.x() * size + this->vertical * point.y() * size;
	return Ray(o, direction);
}

