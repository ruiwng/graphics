#include  "Sphere.h"

bool Sphere::intersect(const Ray& r, Hit& h, float tmin) {
	Vector3f x = r.getOrigin() - center;
	
	float a = r.getDirection().absSquared();
	float b = Vector3f::dot(x, r.getDirection()) * 2;
	float c = Vector3f::dot(x, x) - radius * radius;

	float determinant = b * b - 4 * a * c;
	if (determinant < 0.0f)
		return false;
	float sqrt_determinant = sqrt(determinant);

	float t1 = (-b - sqrt_determinant) / (2 * a);
	bool have_t = false;
	float t;
	if (t1 > tmin && t1 < h.getT()) {
		have_t = true;
		t = t1;
	}
	float t2 = (-b + sqrt_determinant) / (2 * a);
	if (!have_t && t2 > tmin && t2 < h.getT()) {
		have_t = true;
		t = t2;
	}
	if (have_t) {
		Vector3f hit_point = r.getOrigin() + r.getDirection() * t;
		Vector3f dir = hit_point - center;
		dir.normalize();
		h.set(t, material, dir);
		return true;
	}
	return false;
}