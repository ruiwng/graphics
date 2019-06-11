#version 420 core
#extension GL_EXT_gpu_shader4: enable
#extension GL_EXT_geometry_shader4: enable

layout(triangles) in;
layout(triangle_strip, max_vertices=200) out;

in vec3 vNormal[3];

uniform mat4 projection;
uniform float shrink;

out vec3 vNormal1;
out vec3 vPos1;

vec3 V[3];
vec3 CG;

void ProduceVertex(int i){
	vPos1 = CG + shrink * (V[i] - CG);
	vNormal1 = vNormal[i];
	gl_Position = projection * vec4(vPos1, 1.0f);
	EmitVertex();
}

void main(){
	V[0] = gl_PositionIn[0].xyz;
	V[1] = gl_PositionIn[1].xyz;
	V[2] = gl_PositionIn[2].xyz;
	CG = (V[0] + V[1] + V[2]) / 3.0f;
	ProduceVertex(0);
	ProduceVertex(1);
	ProduceVertex(2);
}