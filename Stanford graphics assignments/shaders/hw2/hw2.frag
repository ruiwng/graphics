#version 330

out vec4 fragColor;

uniform float inputTime;

void main() 
{
    // vec4 finalColor = vec4(1);

    // Insert your code for "Slightly-More Advanced Shaders" here.
	float t = (sin(inputTime) + 1.0f) / 2.0f;
	vec3 finalColor = mix(vec3(1.0f, 0.0f, 0.0f), vec3(0.0f, 1.0f, 0.0f), t);
    fragColor = vec4(finalColor, 1.0f);
}
