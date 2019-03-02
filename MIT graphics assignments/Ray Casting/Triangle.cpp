#include "Triangle.h"

Triangle::Triangle(const Vector3f& a, const Vector3f& b, const Vector3f& c, Material* m): Object3D(m), a(a), b(b), c(c){}

bool Triangle::intersect(const Ray& ray, Hit& hit, float tmin) {
	Vector3f x(b - a);
	Vector3f y(c - a);
	Vector3f z(-ray.getDirection());
	Vector3f r(ray.getOrigin() - a);
	Matrix3f M(x, y, z);
	float mDeter = M.determinant();
	//if (mDeter < 0.00001f)
	//	return false;
	float i = Matrix3f(r, y, z).determinant() / mDeter;
	if (i < 0.0f || i > 1.0f)
		return false;
	float j = Matrix3f(x, r, z).determinant() / mDeter;
	if (j < 0.0f || i + j > 1.0f)
		return false;
	float t = Matrix3f(x, y, r).determinant() / mDeter;
	if (t < tmin || t > hit.getT())
		return false;
	Vector3f normal = (1 - i - j)* normals[0] + i * normals[1] + j * normals[2];
	Vector2f texCoord = (1 - i - j) * texCoords[0] + i * texCoords[1] + j * texCoords[2];
	hit.set(t, material, normal);
	hit.setTexCoord(texCoord);
	return true;
}