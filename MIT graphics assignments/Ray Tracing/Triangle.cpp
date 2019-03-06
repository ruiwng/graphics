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
	normal.normalize();
	hit.set(t, material, normal);
	if (hasTex)
	{
		Vector2f texCoord = (1 - i - j) * texCoords[0] + i * texCoords[1] + j * texCoords[2];
		hit.hasTex = true;
		hit.setTexCoord(texCoord);
	}	
	return true;
}

//bool Triangle::intersect(const Ray& ray, Hit& hit, float tmin) {
//	// https://en.wikipedia.org/wiki/M%C3%B6ller%E2%80%93Trumbore_intersection_algorithm
//	const float EPSILON = 0.0000001f;
//	Vector3f edge1, edge2, h, s, q;
//	float g, f, u, v;
//	edge1 = b - a;
//	edge2 = c - a;
//	h = Vector3f::cross(ray.getDirection(), edge2);
//	g = Vector3f::dot(edge1, h);
//	if (g > -EPSILON && g < EPSILON)
//		return false;  // the ray is parallel to this triangle.
//	f = 1.0f / g;
//	s = ray.getOrigin() - a;
//	u = f * (Vector3f::dot(s, h));
//	if (u < 0.0 || u > 1.0f)
//		return false;
//	q = Vector3f::cross(s, edge1);
//	v = f * Vector3f::dot(ray.getDirection(), q);
//	if (v < 0.0f || u + v > 1.0f)
//		return false;
//	// at this stage we can compute t to find out where the intersection point is on the line.
//	float t = f * Vector3f::dot(f, q);
//	if (t < tmin || t > hit.getT())
//		return false;
//
//	Vector3f normal = (1 - u - v)* normals[0] + u * normals[1] + v * normals[2];
//	normal.normalize();
//	hit.set(t, material, normal);
//	if (hasTex)
//	{
//		Vector2f texCoord = (1 - u - v) * texCoords[0] + u * texCoords[1] + v * texCoords[2];
//		hit.hasTex = true;
//		hit.setTexCoord(texCoord);
//	}
//	return true;
//}