#version 420 core

layout (location = 0) in vec3 aVertex;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

out vec3 aPos;

void main(){
	aPos = aVertex;
	gl_Position = projection * view * model * vec4(aVertex, 1.0f);
	gl_Position = gl_Position.xyww;
}