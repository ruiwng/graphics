#version 330 core

struct Material{
	sampler2D diffuse;
	sampler2D specular;
	float shininess;
};

struct DirLight{
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
	
	vec3 direction;
};

struct PointLight{
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
	
	vec3 position;
	
	float constant;
	float linear;
	float quadratic;
};

struct SpotLight{
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

vec3 CalcDirLight(DirLight dirLight, vec3 fragPos, vec3 normalDir, vec3 viewDir);
vec3 CalcPointLight(PointLight pointLight, vec3 normalDir, vec3 viewDir);
vec3 CalcSpotLight(SpotLight spotLight, vec3 fragPos, vec3 normalDir, vec3 viewDir);

#define POINT_LIGHT_NUM  4

uniform Material material;
uniform DirLight dirLight;
uniform PointLight pointLights[POINT_LIGHT_NUM];
uniform SpotLight spotLight;
uniform vec3 cameraPos;

in vec3 wNormal;
in vec3 fragPos;
in vec2 texCoord;

out vec4 fragColor;


void main(){
	vec3 normalDir = normalize(wNormal);
	vec3 viewDir = cameraPos - fragPos;
	viewDir = normalize(viewDir);
	vec3 result = CalcDirLight(dirLight, fragPos, normalDir, viewDir);
	for(int i = 0; i < POINT_LIGHT_NUM; ++i)
		result += CalcPointLight(pointLights[i], normalDir, viewDir);
	result += CalcSpotLight(spotLight, fragPos, normalDir, viewDir);
	fragColor = vec4(result, 1.0f);
}

vec3 CalcDirLight(DirLight dirLight, vec3 fragPos, vec3 normalDir, vec3 viewDir){
	vec3 lightDir = normalize(-dirLight.direction);
	float diff = max(dot(lightDir, normalDir), 0.0f);
	vec3 ambient = dirLight.ambient * texture(material.diffuse, texCoord).rgb;
	vec3 diffuse = dirLight.diffuse * texture(material.diffuse, texCoord).rgb * diff;
	
	vec3 halfDir = normalize(lightDir + viewDir);
	float spec = pow(max(dot(halfDir, normalDir), 0.0f), material.shininess);
	vec3 specular = dirLight.specular * texture(material.specular, texCoord).rgb * spec;
	
	return ambient + diffuse + specular;
}

vec3 CalcPointLight(PointLight pointLight, vec3 normalDir, vec3 viewDir){
	vec3 lightDir = pointLight.position - fragPos;
	float distance = length(lightDir);
	lightDir = normalize(lightDir);
	float diff = max(dot(lightDir, normalDir), 0.0f);
	
	vec3 ambient = pointLight.ambient * texture(material.diffuse, texCoord).rgb;
	vec3 diffuse = pointLight.diffuse * texture(material.diffuse, texCoord).rgb * diff;
	
	vec3 halfDir = normalize(lightDir + viewDir);
	float spec = pow(max(dot(halfDir, normalDir), 0.0f), material.shininess);
	vec3 specular = pointLight.specular * texture(material.specular, texCoord).rgb * spec;
	
	float attenuation = 1 / (pointLight.constant + pointLight.linear * distance + pointLight.quadratic * distance * distance);
	return (ambient + diffuse + specular) * attenuation;
}

vec3 CalcSpotLight(SpotLight spotLight, vec3 fragPos, vec3 normalDir, vec3 viewDir){
	vec3 lightDir = spotLight.position - fragPos;
	float distance = length(lightDir);
	lightDir = normalize(lightDir);
	float diff = max(dot(lightDir, normalDir), 0.0f);
	
	vec3 ambient = spotLight.ambient * texture(material.diffuse, texCoord).rgb;
	vec3 diffuse = spotLight.diffuse * texture(material.diffuse, texCoord).rgb * diff;
	
	vec3 halfDir = normalize(lightDir + viewDir);
	float spec = pow(max(dot(halfDir, normalDir), 0.0f), material.shininess);
	vec3 specular = spotLight.specular * texture(material.specular, texCoord).rgb * spec;
	
	vec3 direction = normalize(spotLight.direction);
	float theta = dot(-lightDir, direction);
	float intensity = (theta - spotLight.outerCutOff) / (spotLight.cutOff - spotLight.outerCutOff);
	intensity = clamp(intensity, 0.0f, 1.0f);
	
	float attenuation = 1 / (spotLight.constant + spotLight.linear * distance + spotLight.quadratic * distance * distance);
	return (ambient + diffuse + specular) * intensity * attenuation;
}