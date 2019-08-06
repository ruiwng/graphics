#ifndef _MATERIAL_H_
#define _MATERIAL_H_

#include "vectors.h"

// ====================================================================
// ====================================================================

// You will extend this class in later assignments

class Ray;
class Hit;

class Material {

public:

	// CONSTRUCTORS & DESTRUCTOR
	Material(const Vec3f &d_color) { diffuseColor = d_color; }
	virtual ~Material() {}

	// ACCESSORS
	virtual Vec3f getDiffuseColor() const { return diffuseColor; }

	virtual Vec3f getSpecularColor() const { return Vec3f(); }

	virtual float getShininess() const { return 0.0f; }

	virtual Vec3f Shade(const Ray &ray, const Hit &hit, const Vec3f &dirToLight, const Vec3f &lightColor) const = 0;
protected:

	// REPRESENTATION
	Vec3f diffuseColor;

};


class PhongMaterial : public Material {
public:
	PhongMaterial(const Vec3f &diffuseColor, const Vec3f &specularColor, float exponent);
	virtual Vec3f Shade(const Ray &ray, const Hit &hit, const Vec3f &dirToLight, const Vec3f &lightColor) const;

	virtual Vec3f getSpecularColor() const { return specularColor; }

	virtual float getShininess() const { return exponent; }
private:
	Vec3f specularColor;
	float exponent;
};

// ====================================================================
// ====================================================================

#endif