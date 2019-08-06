#include "triangle.h"
#include "ray.h"
#include "hit.h"
#include "matrix.h"

#define WIN32

#ifdef WIN32
#include <windows.h>
#endif

#include <GL/GL.h>

Triangle::Triangle(const Vec3f & a, const Vec3f & b, const Vec3f & c, Material * material): a(a), b(b), c(c), material(material){
	Vec3f::Cross3(normal, b - a, c - b);
	normal.Normalize();
}


//bool Triangle::intersect(const Ray &r, Hit &h, float tmin) {
//	Vec3f ab = b - a;
//	Vec3f ac = c - a;
//	Vec3f d = r.getDirection();
//	Vec3f o = r.getOrigin();
//	Vec3f ao = o - a;
//
//	float deter = det3x3(ab.x(), ac.x(), -d.x(),
//		ab.y(), ac.y(), -d.y(), 
//		ab.z(), ac.z(), -d.z());
//
//	float deter2 = det3x3(ao.x(), ac.x(), -d.x(),
//		ao.y(), ac.y(), -d.y(),
//		ao.z(), ac.z(), -d.z());
//
//	float alpha = deter2 / deter;
//	if (alpha < 0.0f || alpha > 1.0f)
//		return false;
//
//	float deter3 = det3x3(ab.x(), ao.x(), -d.x(),
//		ab.y(), ao.y(), -d.y(),
//		ab.z(), ao.z(), -d.z());
//	float beta = deter3 / deter;
//	if (beta < 0.0f || alpha + beta > 1.0f)
//		return false;
//
//	float deter4 = det3x3(ab.x(), ac.x(), ao.x(),
//		ab.y(), ac.y(), ao.y(),
//		ab.z(), ac.z(), ao.z());
//	float t = deter4 / deter;
//	if (t >= tmin && t < h.getT()) {
//		h.set(t, material, normal, r);
//		return true;
//	}
//	return false;
//}


bool Triangle::intersect(const Ray &r, Hit &h, float tmin) {
	// compute the plane-ray intersection.
	float d = -normal.Dot3(a);
	Vec3f dir = r.getDirection();
	Vec3f ori = r.getOrigin();
	float t = (-d - ori.Dot3(normal)) / (dir.Dot3(normal));
	if (t < tmin || t >= h.getT())
		return false;
	// judge whether the intersection is in the triangle or not.
	Vec3f inter = ori + dir * t;
	// drop one dimension.
	Vec2f newA, newB, newC, newInter;

	if (abs(normal.z()) >= abs(normal.x()) && abs(normal.z()) >= abs(normal.y())) {
		newA.Set(a.x(), a.y());
		newB.Set(b.x(), b.y());
		newC.Set(c.x(), c.y());
		newInter.Set(inter.x(), inter.y());
	}
	else if (abs(normal.x()) >= abs(normal.y())) {
		newA.Set(a.y(), a.z());
		newB.Set(b.y(), b.z());
		newC.Set(c.y(), c.z());
		newInter.Set(inter.y(), inter.z());
	}
	else {
		newA.Set(a.z(), a.x());
		newB.Set(b.z(), b.x());
		newC.Set(c.z(), c.x());
		newInter.Set(inter.z(), inter.x());
	}

	Vec2f pointArray[4] = {newA, newB, newC, newA};

	int iterCount = 0;
	for (int i = 0; i < 3; ++i) {
		float a = (newInter.y() - pointArray[i].y()) / (pointArray[i + 1].y() - pointArray[i].y());
		if (a < 0.0f || a > 1.0f)
			continue;
		float b = (1 - a) * pointArray[i].x() + a * pointArray[i + 1].x() - newInter.x();
		if (b >= 0.0f)
			iterCount += 1;
	}
	if (iterCount % 2 == 0)
		return false;
	else
	{
		h.set(t, material, normal, r);
		return true;
	}
}

void Triangle::paint() {
	Vec3f diffuse = material->getDiffuseColor();
	Vec3f specular = material->getSpecularColor();
	GLfloat shininess = material->getShininess();

	GLfloat pDiffuse[] = { diffuse[0], diffuse[1], diffuse[2], 1.0f };
	GLfloat pSpecular[] = { specular[0], specular[1], specular[2], 1.0f };
	glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, pSpecular);
	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, pDiffuse);
	glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, shininess);

	glBegin(GL_TRIANGLES);
	glNormal3f(normal.x(), normal.y(), normal.z());
	glVertex3f(a.x(), a.y(), a.z());
	glVertex3f(b.x(), b.y(), b.z());
	glVertex3f(c.x(), c.y(), c.z());
	glEnd();
}
