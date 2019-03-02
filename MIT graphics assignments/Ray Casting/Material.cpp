#include "Material.h"

Material::Material(const Vector3f& d_color, const Vector3f& s_color, float s) :
	diffuseColor(d_color), specularColor(s_color), shininess(s)
{

}

Vector3f Material::Shade(const Ray& ray, const Hit& hit,
	const Vector3f& dirToLight, const Vector3f& lightColor) {
	Vector3f hitPoint = ray.getOrigin() + ray.getDirection() * hit.getT();
	Vector3f color(0.0f, 0.0f, 0.0f);
	Vector3f normal = hit.getNormal();
	normal.normalize();
	float dot = Vector3f::dot(dirToLight, normal);

	if (dot > 0.0f)
	{
		Vector3f diffuse(diffuseColor);
		if (t.valid())
		{
			Vector2f texCoord = hit.texCoord;
			diffuse = t(texCoord.x(), texCoord.y());
		}
		color += dot * diffuse * lightColor;
	}
	

	Vector3f reflection = 2 * Vector3f::dot(dirToLight, normal) * normal - dirToLight;
	Vector3f dirToView = -ray.getDirection();
	dirToView.normalize();
	dot = Vector3f::dot(dirToView, reflection);
	if (dot > 0.0f)
		color += pow(dot, shininess) * specularColor * lightColor;

	return color;
}