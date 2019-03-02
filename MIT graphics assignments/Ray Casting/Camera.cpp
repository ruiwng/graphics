#include  "Camera.h"

PerspectiveCamera::PerspectiveCamera(const Vector3f& center, const Vector3f& direction, const Vector3f& up, float angle) : center(center), angle(angle) {
	w = -direction.normalized();
	u = Vector3f::cross(up, w);
	u.normalize();
	v = Vector3f::cross(w, u);
	float halfVertical = tan(angle / 2.0f);
	float halfHorizontal = halfVertical;
	lowerLeftCorner = center - halfHorizontal * u - halfVertical * v - w;
	verticalLength = halfVertical * 2;
	horizontalLength = halfHorizontal * 2;
}

Ray PerspectiveCamera::generateRay(const Vector2f& point) {
	Vector3f screenPos = lowerLeftCorner + point.x() * horizontalLength * u + point.y() * verticalLength * v;
	Vector3f dir = (screenPos - center).normalized();
	return Ray(center, dir);
}