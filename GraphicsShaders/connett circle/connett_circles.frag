#version 420 core

uniform float uSide;

in vec2 texCoord;

out vec4 fragColor;

vec3 GetColor(in float t){
	vec3 myColor;
	if(t < 0.0) // black if below bound
		myColor = vec3(0.0f, 0.0f, 0.0f);
	else if(t < 0.2f){
		// purple to blue ramp
		myColor.r = 0.5 * (1.0 - t / 0.2);
		myColor.g = 0.0f;
		myColor.b = 0.5 + (0.5 * t / 0.2);
	}
	else if(t < 0.4f){
		// blue to cyan ramp
		myColor.r = 0.0f;
		myColor.g = (t - 0.2) * 5.0f;
		myColor.b = 1.0f;
	}
	else if(t < 0.6f){
		// cyan to green ramp
		myColor.r = 0.0f; 
		myColor.g = 1.0f;
		myColor.b = (0.6 - t) * 5.0f;
	}else if(t < 0.8f){
		// green to yellow ramp
		myColor.r = (t - 0.6f) * 5.0f;
		myColor.g = 1.0f;
		myColor.b = 0.0f;
	}else if(t < 1.0f){
		// yellow to red ramp
		myColor.r = 1.0f;
		myColor.g = (1.0f - t) * 5.0f;
		myColor.b = 0.0f;
	}
	else{
		myColor = vec3(1.0f, 1.0f, 1.0f);	
	}
	return myColor;
}

void main(){
	vec2 xy = uSide * texCoord.xy;
	float t = dot(xy, xy);
	int c = int(t);
	if((c % 2) != 0) discard;
	t = float(c % 360) / 359.0;
	
	vec3 myColor = GetColor(t);
	fragColor = vec4(myColor, 1.0f);
}