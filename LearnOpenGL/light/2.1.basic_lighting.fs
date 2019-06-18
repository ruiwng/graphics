#version 330 core

vec3 AMBIENT_COLOR = vec3(0.2f, 0.0f, 0.0f);

uniform vec3 objectColor;
uniform vec3 lightColor;
uniform vec3 lightPos;

out vec4 fragColor;
in vec3 wNormal;
in vec3 fragPos;

void main(){
	vec3 normal = normalize(wNormal);
	vec3 lightDir = lightPos - fragPos;
	lightDir = normalize(lightDir);
	float diff = dot(normal, lightDir);
	fragColor = vec4((AMBIENT_COLOR + lightColor * diff) * objectColor, 1.0f);
}