#version 330 core

vec3 AMBIENT_COLOR = vec3(0.2f, 0.0f, 0.0f);
vec3 SPECULAR_COLOR = vec3(0.5f, 0.8f, 0.6f);
uniform vec3 objectColor;
uniform vec3 lightColor;
uniform vec3 lightPos;
uniform vec3 cameraPos;

out vec4 fragColor;
in vec3 wNormal;
in vec3 fragPos;

void main(){
	vec3 normal = normalize(wNormal);
	vec3 lightDir = lightPos - fragPos;
	lightDir = normalize(lightDir);
	float diff = max(dot(normal, lightDir), 0.0f);
	vec3 ambient = AMBIENT_COLOR * objectColor;
	vec3 diffuse = lightColor * diff * objectColor;
	
	vec3 viewDir = cameraPos - fragPos;
	viewDir = normalize(viewDir);
	vec3 reflectDir = reflect(-lightDir, normal);
	float spec = pow(max(dot(reflectDir, viewDir), 0.0f), 32.0f);
	vec3 specular = SPECULAR_COLOR * lightColor * spec;
	fragColor = vec4(ambient + diffuse + specular, 1.0f);
}