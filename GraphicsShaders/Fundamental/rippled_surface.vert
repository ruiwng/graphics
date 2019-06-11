#version 420 core
// procedure model
// z = 0.3 * sin(20.0f * (x * x + y * y))
layout (location = 0) in vec2 aPos;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

out vec3 wPos;
out vec3 wNormal;

void main(){
	float dotProduct = dot(aPos, aPos);
	float cosDotProduct = cos(20.0f * dotProduct);
	vec3 mPos = vec3(aPos, 0.3f * sin(20.0f * dotProduct));
	
	float dzdxZ = 0.6 * aPos.x * 20.0f * cosDotProduct;
	float dzdyZ = 0.6 * aPos.y * 20.0f * cosDotProduct;
	vec3 dzdx = vec3(1.0f, 0.0f, dzdxZ);
	vec3 dzdy = vec3(0.0f, 1.0f, dzdyZ);
	vec3 mNormal = cross(dzdx, dzdy);
	
	wPos = (model * vec4(mPos, 1.0f)).xyz;
	wNormal = (model * vec4(mNormal, 1.0f)).xyz;
	wNormal = normalize(wNormal);
	
	gl_Position = projection * view * vec4(wPos, 1.0f);
}