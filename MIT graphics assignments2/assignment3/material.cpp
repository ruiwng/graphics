#include "material.h"

#include "ray.h"
#include "hit.h"

#include <algorithm>


PhongMaterial::PhongMaterial(const Vec3f &diffuseColor, const Vec3f &specularColor, float exponent) :Material(diffuseColor), specularColor(specularColor), exponent(exponent) {}

Vec3f PhongMaterial::Shade(const Ray &ray, const Hit &hit, const Vec3f &dirToLight, const Vec3f &lightColor) const {
	Vec3f viewDir = ray.getDirection();
	viewDir.Negate();
	viewDir.Normalize();
	Vec3f normal = hit.getNormal();
	normal.Normalize();
	float diff = std::max(0.0f, normal.Dot3(dirToLight));
	Vec3f diffuse = getDiffuseColor() * lightColor * diff;
	Vec3f halfDir = viewDir + dirToLight;
	halfDir.Normalize();
	float spec = std::pow(std::max(0.0f, halfDir.Dot3(normal)), exponent);
	Vec3f specular = getSpecularColor() * lightColor * spec;

	return diffuse + specular;
}