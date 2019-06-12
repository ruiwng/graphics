#version 420 core

in vec3 aPos;
out vec4 fragColor;
uniform samplerCube skyBox;

void main(){
	vec3 color = texture(skyBox, aPos).rgb;
	fragColor = vec4(color, 1.0f);
}