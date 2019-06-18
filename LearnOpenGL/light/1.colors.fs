#version 330 core

uniform vec3 objectColor;
uniform vec3 lightColor;

out vec4 fragColor;

void main(){
	fragColor = vec4(objectColor * lightColor, 1.0f);
}