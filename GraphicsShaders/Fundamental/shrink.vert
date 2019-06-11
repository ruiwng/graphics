#version 420 core

layout (location = 0) in vec3 aVertex;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

out vec3 vNormal;

void main(){
	gl_Position = view * model * vec4(aVertex, 1.0f);
	vNormal = (view * model * vec4(aVertex, 0.0f)).xyz;
	vNormal = normalize(vNormal);
}