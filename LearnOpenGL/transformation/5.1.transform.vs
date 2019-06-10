#version 330 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aColor;
layout (location = 2) in vec2 aTexCoord;

out vec2 texCoord;
out vec3 color;

uniform mat4 transform;

void main(){
	texCoord = aTexCoord;
	color = aColor;
	gl_Position = transform * vec4(aPos, 1.0f);
}