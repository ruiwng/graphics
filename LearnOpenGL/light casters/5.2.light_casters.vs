#version 330 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoord;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

out vec3 wNormal;
out vec3 fragPos;
out vec2 texCoord;

void main(){
	texCoord = aTexCoord;
	wNormal = (model * vec4(aNormal, 0.0f)).xyz;
	wNormal = normalize(wNormal);
	fragPos = (model * vec4(aPos, 1.0f)).xyz;
	gl_Position = projection * view * vec4(fragPos, 1.0f);
}