#version 330 core
layout (location = 0) in vec3 pos;
layout (location = 1) in vec3 normal;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

out vec3 frag_pos;
out vec3 frag_normal;

void main(){
	frag_pos = pos;
	vec4 new_normal = model * vec4(normal, 1.0f);
	frag_normal = normalize(new_normal.xyz);
	gl_Position = projection * view * model * vec4(pos.x, pos.y, pos.z, 1.0);
}