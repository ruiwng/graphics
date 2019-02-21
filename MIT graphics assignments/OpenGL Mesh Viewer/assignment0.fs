#version 330 core
out vec4 FragColor;

in vec3 frag_pos;
in vec3 frag_normal;

uniform vec3 camera_pos;
uniform vec3 light_pos;
uniform vec3 model_color;
uniform vec3 light_color;

void main(){
	vec3 light_color = vec3(1.0f, 1.0f, 1.0f);
	vec3 normal = normalize(frag_normal);
	vec3 ambient = vec3(1.0f, 1.0f, 1.0f);
	vec3 light_dir = normalize(light_pos - frag_pos);
	float x = dot(light_dir, normal);
	x = max(x, 0.0f);
	vec3 diffuse = light_color * x;
	vec3 camera_dir = normalize(camera_pos - frag_pos);
	vec3 reflect_dir = reflect(-light_dir, normal);
	reflect_dir = normalize(reflect_dir);
	vec3 half_angle = light_dir + camera_dir;
	half_angle = normalize(half_angle);
	float y = dot(half_angle, normal);
	y = max(y, 0.0f);
	vec3 specular = light_color * pow(y, 32);
	vec3 color = (ambient + diffuse + specular) * model_color;
	FragColor = vec4(pow(color.r, 2.2f), pow(color.g, 2.2f), pow(color.b, 2.2f), 1.0f);
}