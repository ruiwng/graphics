#include "RayTracer.h"
#include "Camera.h"
#include "Ray.h"
#include "Hit.h"
#include "Group.h"
#include "Material.h"
#include "Light.h"

#define EPSILON 0.001

//IMPLEMENT THESE FUNCTIONS
//These function definitions are mere suggestions. Change them as you like.
Vector3f mirrorDirection(const Vector3f& normal, const Vector3f& incoming)
{
	Vector3f n = normal.normalized();
	Vector3f out = incoming - 2 * Vector3f::dot(incoming, normal) * normal;
	return out.normalized();
}

bool transmittedDirection(const Vector3f& normal, const Vector3f& incoming,
	float index_n, float index_nt,
	Vector3f& transmitted)
{
	Vector3f in = incoming.normalized();
	Vector3f n = normal.normalized();
	float dot = Vector3f::dot(in, n);
	if (dot > 0.0f)
	{
		dot = -dot;
		n = -n;
	}	
	float ratio = index_n / index_nt;
	float det = 1 - ratio * ratio * (1 - dot * dot);
	if(det < 0.0f)
		return false;
	transmitted = ratio * (in - dot * n) - sqrt(det) * n;
	transmitted.normalize();
	return true;
}

RayTracer::RayTracer(SceneParser * scene, int max_bounces
	//more arguments if you need...
) :
	m_scene(scene)

{
	g = scene->getGroup();
	camera = scene->getCamera();
	m_maxBounces = max_bounces;
}

RayTracer::~RayTracer()
{
}

Vector3f RayTracer::traceRay(Ray& ray, float tmin, int bounces,
	float refr_index, Hit& hit, bool shadows) const
{
	//hit = Hit(FLT_MAX, NULL, Vector3f(0, 0, 0));
	bool isIntersect = g->intersect(ray, hit, tmin);
	if (!isIntersect) {
		return m_scene->getBackgroundColor(ray.getDirection());
	}
	Vector3f hitPoint = ray.getOrigin() + ray.getDirection() * hit.getT();
	Material *hitMaterial = hit.getMaterial();
	int numLight = m_scene->getNumLights();
	Vector3f color = m_scene->getAmbientLight() * hitMaterial->getDiffuseColor();

	for (int i = 0; i < numLight; ++i) {
		Light *light = m_scene->getLight(i);
		Vector3f lightColor, direction;
		float distanceToLight;
		light->getIllumination(hitPoint, direction, lightColor, distanceToLight);
		Hit lightHit(distanceToLight, NULL, Vector3f(0.0f, 0.0f, 0.0f));
		Ray lightRay(hitPoint, direction);
		isIntersect = g->intersect(lightRay, lightHit, EPSILON);
		if (!shadows || !isIntersect)
			color += hitMaterial->Shade(ray, hit, direction, lightColor);
	}

	if (bounces > 0) {
		// reflection
		Vector3f reflectDir = mirrorDirection(hit.getNormal(), ray.getDirection());
		Ray reflectRay(hitPoint, reflectDir);
		Hit reflectHit;
		Vector3f reflectColor = traceRay(reflectRay, EPSILON, bounces - 1,
			refr_index, reflectHit, shadows);

		// refraction
		float in=refr_index, out;
		float refractionIndex = hitMaterial->getRefractionIndex();
		if (refractionIndex > 0.0f) {
			if (Vector3f::dot(ray.getDirection(), hit.getNormal()) > 0) {
				// ray is inside object
				out = 1.0f;
			}
			else
				out = refractionIndex;
			Vector3f refractDir;
			bool isTransmitted = transmittedDirection(hit.getNormal(), ray.getDirection(), in, out, refractDir);
			if (isTransmitted) {
	/*			Vector3f inDir = ray.getDirection();
				Vector3f tDir = refractDir;
				if (tDir.x() * inDir.x() < 0 || tDir.y() * inDir.y() < 0 || tDir.z() * inDir.z() < 0)
				{
					cout << "in " << inDir.x() << " " << inDir.y() << " " << inDir.z() << endl;
					cout << "t " << tDir.x() << " " << tDir.y() << " " << tDir.z() << endl;
				}*/
				Hit refractHit;
				Ray refractRay(hitPoint, refractDir);
				Vector3f refractColor = traceRay(refractRay, EPSILON, bounces - 1, out, refractHit, shadows);
				float r0 = pow((out - in) / (out + in), 2.0f);
				float c;
				if (in <= out)
					c = abs(Vector3f::dot(ray.getDirection().normalized(), hit.getNormal().normalized()));
				else
					c = abs(Vector3f::dot(refractDir.normalized(), hit.getNormal().normalized()));
				float r = r0 + (1 - r0) * pow(1 - c, 5.0f);
				color += r * reflectColor * hitMaterial->getSpecularColor() + (1 - r) * refractColor;
			}
			else {
				color += reflectColor * hitMaterial->getSpecularColor();
			}
		}
		else
			color += reflectColor * hitMaterial->getSpecularColor();
		
	}

	return color;
}
