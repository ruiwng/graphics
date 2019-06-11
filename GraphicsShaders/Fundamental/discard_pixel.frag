#version 420 core
out vec4 fragColor;

vec3 LIGHT_POS = vec3(5.0f, 5.0f, 2.0f);
vec3 AMBIENT_COLOR = vec3(0.2f, 0.2f, 0.2f);
vec3 LIGHT_COLOR = vec3(1.0f, 1.0f, 0.0f);
vec3 MATERIAL_COLOR = vec3(1.0f, 0.5f, 0.0f);

in vec3 wPos;
in vec3 wNormal;

uniform float frequency;
uniform float density;

void main(){
	float s = acos(wPos.y) * frequency;
	float t = atan(wPos.z, wPos.x) * frequency;
	if(fract(s) > density && fract(t) > density)
		discard;
	vec3 normal = normalize(wNormal);
	vec3 fragToLight = LIGHT_POS - wPos;
	fragToLight = normalize(fragToLight);
	vec3 diffuse = MATERIAL_COLOR * LIGHT_COLOR * clamp(dot(fragToLight, wNormal), 0.0f, 1.0f);
	vec3 ambient = AMBIENT_COLOR * MATERIAL_COLOR;
	fragColor = vec4(diffuse + ambient, 1.0f);
}