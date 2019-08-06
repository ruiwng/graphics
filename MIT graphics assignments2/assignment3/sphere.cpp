#include "sphere.h"
#include "ray.h"
#include "hit.h"

#define _USE_MATH_DEFINES
#include <math.h>

#define WIN32

#ifdef WIN32
#include <windows.h>
#endif

#include <GL/GL.h>

int thetaSteps = 10;
int phiSteps = 5;
bool enableGouraud = false;


Sphere::Sphere(const Vec3f &center, float radius, Material *material):Object3D(material), center(center), radius(radius), isPaintInit(false){}


// algebra method
bool Sphere::intersect(const Ray &r, Hit &h, float tmin) {
	Vec3f CtoO = r.getOrigin() - center;
	Vec3f dir = r.getDirection();

	float a = dir.Dot3(dir);
	float b = 2 * CtoO.Dot3(dir);
	float c = CtoO.Dot3(CtoO) - radius * radius;
	float delta = b * b - 4 * a * c;
	
	if (delta < 0.0f)
		return false;
	float deltaRoot = sqrt(delta);

	float t1 = (-b - deltaRoot) / (2 * a);
	bool x1 = t1 >= tmin;
	float x = h.getT();
	bool x2 = t1 < h.getT();
	if (t1 >= tmin && t1 < h.getT()) {
		Vec3f normal = r.getOrigin() + t1 * r.getDirection() - center;
		normal.Normalize();
		h.set(t1, material, normal, r);
		return true;
	}
	
	float t2 = (-b + deltaRoot) / (2 * a);
	if (t2 >= tmin && t1 < h.getT()) {
		Vec3f normal = r.getOrigin() + t2 * r.getDirection() - center;
		normal.Normalize();
		h.set(t2, material, normal, r);
		return true;
	}
	return false;
}

// geometry method
//bool Sphere::intersect(const Ray &r, Hit &h, float tmin) {
//	Vec3f OtoC = center - r.getOrigin();
//	Vec3f dir = r.getDirection();
//	float minT = OtoC.Dot3(dir);
//	Vec3f minPoint = r.getOrigin() + minT * r.getDirection();
//	
//	float r2 = radius * radius;
//	Vec3f a = minPoint - center;
//	float a2 = a.Dot3(a);
//	if (a2 > r2)
//		return false;
//	float b = sqrt(r2 - a2);
//	float deltaT = b / dir.Length();
//	
//	float t1 = minT - deltaT;
//	if (t1 >= tmin && t1 < h.getT()) {
//		Vec3f normal = r.getOrigin() + t1 * r.getDirection() - center;
//		normal.Normalize();
//		h.set(t1, material, normal, r);
//		return true;
//	}
//
//	float t2 = minT + deltaT;
//	if (t2 >= tmin && t2 < h.getT()) {
//		Vec3f normal = r.getDirection() + t2 * r.getDirection() - center;
//		normal.Normalize();
//		h.set(t2, material, normal, r);
//		return true;
//	}
//	return false;
//}

bool Sphere::intersectAll(const Ray &r, std::vector<HitPair> &hitArray) {
	Vec3f OtoC = center - r.getOrigin();
	Vec3f dir = r.getDirection();
	float minT = OtoC.Dot3(dir);
	Vec3f minPoint = r.getOrigin() + minT * r.getDirection();
		
	float r2 = radius * radius;
	Vec3f a = minPoint - center;
	float a2 = a.Dot3(a);
	if (a2 > r2)
		return false;
	float b = sqrt(r2 - a2);
	float deltaT = b / dir.Length();
		
	float t1 = minT - deltaT;
	Vec3f normal = r.getOrigin() + t1 * r.getDirection() - center;
	normal.Normalize();
	Hit h1(t1, material, normal);
	
	float t2 = minT + deltaT;
	normal = r.getDirection() + t2 * r.getDirection() - center;
	normal.Normalize();
	Hit h2(t2, material, normal);

	hitArray.push_back(make_pair(h1, h2));
	return true;
}

void Sphere::paint() {
	if (!isPaintInit) {
		std::vector<std::vector<Vec3f>> temp(phiSteps + 1, std::vector<Vec3f>(thetaSteps + 1));
		pointArray.swap(temp);
		normalArray = pointArray;
		float phiStepDistance = M_PI / phiSteps;
		float thetaStepDistance = 2.0f * M_PI / thetaSteps;
		float currentPhi = -0.5f * M_PI;
		for (int i = 0; i <= phiSteps; ++i) {
			float currentTheta = 0.0f;
			float currentPhiSin = sin(currentPhi);
			float currentPhiCos = cos(currentPhi);
			for (int j = 0; j <= thetaSteps; ++j) {
				Vec3f point(currentPhiCos * cos(currentTheta), currentPhiSin, currentPhiCos * sin(currentTheta));
				normalArray[i][j] = point;
				point *= radius;
				point += center;
				pointArray[i][j] = point;
				currentTheta += thetaStepDistance;
			}
			currentPhi += phiStepDistance;
		}
		isPaintInit = true;
	}

	Vec3f diffuse = material->getDiffuseColor();
	Vec3f specular = material->getSpecularColor();
	GLfloat shininess = material->getShininess();

	
	GLfloat pDiffuse[] = { diffuse[0], diffuse[1], diffuse[2], 1.0f};
	GLfloat pSpecular[] = { specular[0], specular[1], specular[2], 1.0f};
	glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, pSpecular);
	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, pDiffuse);
	glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, shininess);

	glBegin(GL_QUADS);
	if (enableGouraud) {
		for (int i = 0; i < phiSteps; ++i) {
			for (int j = 0; j < thetaSteps; ++j) {
				const Vec3f& a = pointArray[i][j];
				const Vec3f& b = pointArray[i][j + 1];
				const Vec3f& c = pointArray[i + 1][j + 1];
				const Vec3f& d = pointArray[i + 1][j];

				const Vec3f& na = normalArray[i][j];
				const Vec3f& nb = normalArray[i][j + 1];
				const Vec3f& nc = normalArray[i + 1][j + 1];
				const Vec3f& nd = normalArray[i + 1][j];

				glNormal3f(na.x(), na.y(), na.z());
				glVertex3f(a.x(), a.y(), a.z());
				glNormal3f(nb.x(), nb.y(), nb.z());
				glVertex3f(b.x(), b.y(), b.z());
				glNormal3f(nc.x(), nc.y(), nc.z());
				glVertex3f(c.x(), c.y(), c.z());
				glNormal3f(nd.x(), nd.y(), nd.z());
				glVertex3f(d.x(), d.y(), d.z());
			}
		}
	}
	else {
		for (int i = 0; i < phiSteps; ++i) {
			for (int j = 0; j < thetaSteps; ++j) {
				const Vec3f& a = pointArray[i][j];
				const Vec3f& b = pointArray[i][j + 1];
				const Vec3f& c = pointArray[i + 1][j + 1];
				const Vec3f& d = pointArray[i + 1][j];

				Vec3f normal;
				Vec3f ab = b - a;
				Vec3f bc = c - b;
				Vec3f::Cross3(normal, bc, ab);

				glNormal3f(normal.x(), normal.y(), normal.z());
				glVertex3f(a.x(), a.y(), a.z());
				glVertex3f(b.x(), b.y(), b.z());
				glVertex3f(c.x(), c.y(), c.z());
				glVertex3f(d.x(), d.y(), d.z());
			}
		}
	}
	glEnd();
}