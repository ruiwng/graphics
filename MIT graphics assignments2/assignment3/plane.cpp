#include "plane.h"
#include "ray.h"
#include "hit.h"

#define WIN32

#ifdef WIN32
#include <windows.h>
#endif

#include <GL/GL.h>


Plane::Plane(Vec3f &normal, float d, Material *material): Object3D(material), normal(normal), d(d), isPaintInit(false) {
	normal.Normalize();
}

bool Plane::intersect(const Ray &r, Hit &h, float tmin) {
	Vec3f orig = r.getOrigin();
	Vec3f dir = r.getDirection();

	float t = (d - orig.Dot3(normal)) / (dir.Dot3(normal));
	if (t >= tmin && t < h.getT()) {
		h.set(t, material, normal, r);
		return true;
	}
	return false;
}


void Plane::paint() {
	if (!isPaintInit) {
		Vec3f ori = d * normal;
		Vec3f x, y;
		if (abs(normal.x()) > abs(normal.y()))
			x = Vec3f(-normal.z(), 0.0f, normal.x());
		else
			x = Vec3f(0.0f, -normal.z(), normal.y());
		x.Normalize();
		Vec3f::Cross3(y, normal, x);

		quad[0] = ori + x * -1e5 + y * -1e5;
		quad[1] = ori + x * 1e5 + y * -1e5;
		quad[2] = ori + x * 1e5 + y * 1e5;
		quad[3] = ori + x * -1e5 + y * 1e5;
		isPaintInit = true;
	}
	
	Vec3f diffuse = material->getDiffuseColor();
	Vec3f specular = material->getSpecularColor();
	GLfloat shininess = material->getShininess();


	GLfloat pDiffuse[] = { diffuse[0], diffuse[1], diffuse[2], 1.0f };
	GLfloat pSpecular[] = { specular[0], specular[1], specular[2], 1.0f };
	glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, pSpecular);
	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, pDiffuse);
	glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, shininess);

	glBegin(GL_QUADS);
	for (int i = 0; i < 4; ++i) {
		glVertex3f(quad[i].x(), quad[i].y(), quad[i].z());
	}
	glEnd();
}