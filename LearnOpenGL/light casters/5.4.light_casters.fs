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
	float outerCutOff;
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
	float epsilon = light.cutOff - light.outerCutOff;
	float intensity = clamp((theta - light.outerCutOff) / epsilon, 0.0f, 1.0f);
	diffuse *= intensity;
	specular *= intensity;
	fragColor = vec4((ambient + diffuse + specular) * attenuation, 1.0f);
}