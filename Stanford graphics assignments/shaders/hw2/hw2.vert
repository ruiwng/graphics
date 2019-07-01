#version 330

layout(location = 0) in vec4 vertexPosition;

uniform float inputTime;

void main()
{
    vec4 modifiedVertexPosition = vertexPosition;
    // Insert your code for "Slightly-More Advanced Shaders" here.
	modifiedVertexPosition.y = modifiedVertexPosition.y + (sin(inputTime) - 1.0f) / 2.0f;
    gl_Position = modifiedVertexPosition;
}
