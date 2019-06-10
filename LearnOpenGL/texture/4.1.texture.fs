#version 330 core

in vec2 texCoord;
in vec3 color;
out vec4 fragColor;

uniform sampler2D texture1;

void main(){
	vec3 newColor = texture(texture1, texCoord).rgb * color;
	fragColor = vec4(newColor, 1.0f);
}