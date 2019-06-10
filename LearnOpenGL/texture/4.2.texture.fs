#version 330 core

in vec2 texCoord;
in vec3 color;
out vec4 fragColor;

uniform sampler2D texture1;
uniform sampler2D texture2;

void main(){
	fragColor = mix(texture(texture1, texCoord), texture(texture2, texCoord), 0.2);
}