#version 420 core
out vec4 fragColor;

vec3 LIGHT_POS = vec3(5.0f, 5.0f, 2.0f);
vec3 AMBIENT_COLOR = vec3(0.2f, 0.2f, 0.2f);
vec3 LIGHT_COLOR = vec3(1.0f, 1.0f, 0.0f);
vec3 MATERIAL_COLOR = vec3(1.0f, 0.5f, 0.0f);

in vec3 vPos1;
in vec3 vNormal1;

uniform mat4 view;

void main(){
	vec3 normal = normalize(vNormal1);
	vec3 lightPos = (view * vec4(LIGHT_POS, 1.0f)).xyz;
	vec3 fragToLight = lightPos - vPos1;
	fragToLight = normalize(fragToLight);
	vec3 diffuse = MATERIAL_COLOR * LIGHT_COLOR * clamp(dot(fragToLight, normal), 0.0f, 1.0f);
	vec3 ambient = AMBIENT_COLOR * MATERIAL_COLOR;
	fragColor = vec4(ambient + diffuse, 1.0f);
}