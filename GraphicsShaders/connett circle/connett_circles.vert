#version 420 core

layout (location = 0) in vec3 aVertex;
layout (location = 1) in vec2 aTexCoord;

out vec2 texCoord;

void main(){
	texCoord = aTexCoord;
	gl_Position = vec4(aVertex * 1.5f, 1.0f);
}