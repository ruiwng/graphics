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
	vec3 direction;
	float cutOff;
	float constant;
	float linear;
	float quadratic;
};

uniform vec3 cameraPos;
uniform Material material;
uniform Light light;

void main(){
	vec3 normal = normalize(wNormal);
	vec3 lightDir = light.position - fragPos;
	float lightDistance = length(lightDir);
	lightDir = normalize(lightDir);
	float attenuation = 1.0f / (light.constant + light.linear * lightDistance + light.quadratic * lightDistance * lightDistance);
	vec3 ambient = light.ambient * texture(material.diffuse, texCoord).rgb;
	
	float diff = max(dot(normal, lightDir), 0.0f);
	vec3 diffuse = texture(material.diffuse, texCoord).rgb * light.diffuse * diff;
	
	vec3 reflectDir = reflect(-lightDir, normal);
	reflectDir = normalize(reflectDir);
	vec3 viewDir = cameraPos - fragPos;
	viewDir = normalize(viewDir);
	float spec = pow(max(dot(viewDir, reflectDir), 0.0f), material.shininess);
	vec3 specular = texture(material.specular, texCoord).rgb * light.specular * spec;
	
	float theta = dot(-lightDir, normalize(light.direction));
	if (theta >= light.cutOff)
		fragColor = vec4(ambient + (diffuse + specular) * attenuation, 1.0f);
	else
		fragColor = vec4(ambient, 1.0f);
}