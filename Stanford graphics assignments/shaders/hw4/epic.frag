#version 330

in vec4 fragmentColor;
in vec4 vertexWorldPosition;
in vec3 vertexWorldNormal;

out vec4 finalColor;

uniform InputMaterial {
    float matRoughness;
    vec4 matSpecular;
    float matMetallic;
} material;

struct LightProperties {
    vec4 diffuseColor;
    vec4 specularColor;
    vec4 directionalLightDir;
    float spotInnerConeAngleDegrees;
    float spotOuterConeAngleDegrees;
};
uniform LightProperties genericLight;

struct PointLight {
    vec4 pointPosition;
};
uniform PointLight pointLight;

uniform vec4 cameraPosition;

uniform float constantAttenuation;
uniform float linearAttenuation;
uniform float quadraticAttenuation;

uniform int lightingType;

const float PI = 3.1415926535;

float computeD(float roughness, vec4 N, vec4 H){
	float a = roughness * roughness;
	float aSquare = a * a;
	float NdH = max(dot(N, H), 0.0f);
	float denominator = NdH * NdH * (aSquare - 1) + 1;
	denominator = PI * denominator * denominator;
	return aSquare / denominator;
}

float computeG1(float roughness, vec4 N, vec4 dir){
	float k = pow(roughness + 1, 2) / 8.0f;
	float NdD = max(dot(N, dir), 0.0f);
	return NdD / (NdD * (1 - k) + k);
}

float computeG(float roughness, vec4 N, vec4 L, vec4 V){
	float x = computeG1(roughness, N, L);
	float y = computeG1(roughness, N, V);
	return x * y;
}

vec4 computeF(vec4 cSpec, vec4 V, vec4 H){
	float VdH = max(dot(V, H), 0.0f);
	float power = (-5.55473 * VdH - 6.98316) * VdH;
	return cSpec + (1 - cSpec) * pow(2, power);
}

void computeBRDF(vec4 N, vec4 L, vec4 V, out vec4 d, out vec4 s){
	float NdL = max(0, dot(N, L));
	float NdV = max(0, dot(N, V));
	vec4 H = L + V;
	H = normalize(H);
	
	vec4 cDiff = (1 - material.matMetallic) * fragmentColor;
	vec4 cSpec = mix(0.08f * material.matSpecular, fragmentColor, material.matMetallic);
	
	d = cDiff / PI;
	
	float D = computeD(material.matRoughness, N, H);
	float G = computeG(material.matRoughness, N, L, V);
	vec4 F = computeF(cSpec, V, H);
	
	vec4 numerator = D * F * G;
	
	float denominator = 4 * NdL * NdV;
	s = numerator / max(0.001f, denominator);
}

vec4 pointLightSubroutine(vec4 N, vec4 worldPosition, vec3 worldNormal)
{
    // Direction from the surface to the point light
    vec4 L = normalize(pointLight.pointPosition - vertexWorldPosition);
	vec4 V = normalize(cameraPosition - worldPosition);
    float NdL = max(0, dot(N, L));
	
    // Insert code for Section 3.2 here.
	vec4 d, s;
	computeBRDF(N, L, V, d, s);
	vec4 brdf = NdL * (d * genericLight.diffuseColor + s * genericLight.specularColor);
    return brdf;
}

vec4 directionalLightSubroutine(vec4 N, vec4 worldPosition, vec3 worldNormal)
{
    // Insert code for Section 3.3 here.
	vec4 L = normalize(-genericLight.directionalLightDir);
	vec4 V = normalize(cameraPosition - worldPosition);
	float NdL = max(0, dot(N, L));
	
	vec4 d, s;
	computeBRDF(N, L, V, d, s);
	vec4 brdf = NdL * (d * genericLight.diffuseColor + s * genericLight.specularColor);
    return brdf;
}

vec4 hemisphereLightSubroutine(vec4 N, vec4 worldPosition, vec3 worldNormal)
{
    // Insert code for Section 3.4 here.
	vec4 V = normalize(cameraPosition - worldPosition);
	float weight = (dot(N, vec4(0.0f, 1.0f, 0.0f, 0.0f)) + 1.0f) / 2.0f;
	vec4 lightColor = mix(genericLight.diffuseColor, genericLight.specularColor, weight);
	vec4 L = N;
	float NdL = max(0, dot(N, L));
	
	vec4 d, s;
	computeBRDF(N, L, V, d, s);
    return NdL * d * lightColor;
}

vec4 spotLightSubroutine(vec4 N, vec4 worldPosition, vec3 worldNormal)
{
    // Insert code for Section 3.5 here.
	vec4 V = normalize(cameraPosition - worldPosition);
	vec4 L = normalize(pointLight.pointPosition - worldPosition);
	float NdL = max(0, dot(N, L));
	
	vec4 lightDir = normalize(genericLight.directionalLightDir);
	
	float theta = dot(lightDir, -L);
	float innerCutOff = cos(genericLight.spotInnerConeAngleDegrees * PI / 180.0f);
	float outerCutOff = cos(genericLight.spotOuterConeAngleDegrees * PI / 180.0f);
	
	float density = (theta - outerCutOff) / (innerCutOff - outerCutOff);
	density = clamp(density, 0.0f, 1.0f);
	
	vec4 d, s;
	computeBRDF(N, L, V, d, s);
    return NdL * (d * genericLight.diffuseColor + s * genericLight.specularColor) * density;
}

vec4 globalLightSubroutine(vec4 worldPosition, vec3 worldNormal)
{
    return vec4(0.0);
}

vec4 attenuateLight(vec4 originalColor)
{
    float lightDistance = length(pointLight.pointPosition - vertexWorldPosition);
    float attenuation = 1.0 / (constantAttenuation + lightDistance * linearAttenuation + lightDistance * lightDistance * quadraticAttenuation);
    return originalColor * attenuation;
}

void main()
{
    // Normal to the surface
    vec4 N = vec4(normalize(vertexWorldNormal), 0.f);

    vec4 lightingColor = vec4(0);
    if (lightingType == 0) {
        lightingColor = globalLightSubroutine(vertexWorldPosition, vertexWorldNormal);
    } else if (lightingType == 1) {
        lightingColor = attenuateLight(pointLightSubroutine(N, vertexWorldPosition, vertexWorldNormal));
    } else if (lightingType == 2) {
        lightingColor = directionalLightSubroutine(N, vertexWorldPosition, vertexWorldNormal);
    } else if (lightingType == 3) {
        lightingColor = hemisphereLightSubroutine(N, vertexWorldPosition, vertexWorldNormal);
    } else if (lightingType == 5) {
        lightingColor = attenuateLight(spotLightSubroutine(N, vertexWorldPosition, vertexWorldNormal));
    }
    finalColor = lightingColor * fragmentColor;
    finalColor.a = 1.0;
}

