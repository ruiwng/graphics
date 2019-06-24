#version 330 core

out vec4 fragColor;

in vec3 wNormal;
in vec3 fragPos;
in vec2 texCoord;

struct Material{
	sampler2D diffuse;
	sampler2D specular;
	float shininess;
};

struct Light{
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
	vec3 position;
};

uniform vec3 cameraPos;
uniform Material material;
uniform Light light;

void main(){
	vec3 normal = normalize(wNormal);
	vec3 lightDir = light.position - fragPos;
	lightDir = normalize(lightDir);
	
	vec3 ambient = light.ambient * texture(material.diffuse, texCoord).rgb;
	
	float diff = max(dot(normal, lightDir), 0.0f);
	vec3 diffuse = texture(material.diffuse, texCoord).rgb * light.diffuse * diff;
	
	vec3 reflectDir = reflect(-lightDir, normal);
	reflectDir = normalize(reflectDir);
	vec3 viewDir = cameraPos - fragPos;
	viewDir = normalize(viewDir);
	float spec = pow(max(dot(viewDir, reflectDir), 0.0f), material.shininess);
	vec3 specular = texture(material.specular, texCoord).rgb * light.specular * spec;
	
	fragColor = vec4(ambient + diffuse + specular, 1.0f);
}