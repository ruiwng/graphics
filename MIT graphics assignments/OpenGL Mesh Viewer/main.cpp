#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <iostream>
#include <vector>

#include  <stdio.h>
#include  "shader.h"

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void processInput(GLFWwindow *window);

// settings
unsigned int SCR_WIDTH = 800;
unsigned int SCR_HEIGHT = 800;

glm::vec3 light_pos = glm::vec3(1.0f, 1.0f, 5.0f);

glm::vec3 camera_pos = glm::vec3(0.0f, 0.0f, 5.0f);
glm::vec3 camera_target = glm::vec3(0.0f, 0.0f, 0.0f);
glm::vec3 camera_up = glm::vec3(0.0f, 1.0f, 0.0f);
float fov = 45.0f;

glm::vec3 model_color_array[] = {glm::vec3(0.5f, 0.5f, 0.9f), glm::vec3(0.9f, 0.5f, 0.5f), glm::vec3(0.5f, 0.9f, 0.5f)};
int model_color_index = 0;
float change_color_time = 0.0f;
bool is_change_color = false;

float delta_time = 0.0f;
float last_time = 0.0f;

bool is_rotate = false;
float rotate_radian = 0.0f;


bool parse_obj(const char *obj_name, std::vector<float> &vertices, std::vector<float> &normals, std::vector<unsigned int> &vertex_indices, std::vector<unsigned int> & normal_indices) {
	FILE *f = fopen(obj_name, "r");
	if (f == NULL) {
		printf("open file failed!!!!!!!!!!!!!!!!!!!!!");
		return false;
	}
	char line[1024];

	while (!feof(f))
	{
		fgets(line, 1024, f);
		char type[10];
		if (strncmp(line, "v ", 2) == 0) {
			float x, y, z;
			sscanf(line, "%s %f %f %f", type, &x, &y, &z);
			vertices.push_back(x);
			vertices.push_back(y);
			vertices.push_back(z);
		} 
		else if (strncmp(line, "vn ", 3) == 0) {
			float nx, ny, nz;
			sscanf(line, "%s %f %f %f", type, &nx, &ny, &nz);
			normals.push_back(nx);
			normals.push_back(ny);
			normals.push_back(nz);
		}
		else if (strncmp(line, "f ", 2) == 0) {
			unsigned int ix, iy, iz;
			unsigned int inx, iny, inz;
			unsigned int u, v, w;
			sscanf(line, "%s %u/%u/%u %u/%u/%u %u/%u/%u", type, &ix, &u, &inx, &iy, &v, &iny, &iz, &w, &inz);
			vertex_indices.push_back(ix - 1);
			vertex_indices.push_back(iy - 1);
			vertex_indices.push_back(iz - 1);
			normal_indices.push_back(inx - 1);
			normal_indices.push_back(iny - 1);
			normal_indices.push_back(inz - 1);
		}
	}
	return true;
}

std::vector<float> generate_vertex_attrib(const std::vector<float> &vertices, const std::vector<float> &normals, const std::vector<unsigned int> &vertex_indices, const std::vector<unsigned int> & normal_indices) {
	std::vector<float> vertex_attrib;
	int size = vertex_indices.size();
	for (int i = 0; i < size; ++i) {
		int vertex_index = vertex_indices[i] * 3;
		int normal_index = normal_indices[i] * 3;
		vertex_attrib.push_back(vertices[vertex_index]);
		vertex_attrib.push_back(vertices[vertex_index + 1]);
		vertex_attrib.push_back(vertices[vertex_index + 2]);
		vertex_attrib.push_back(normals[normal_index]);
		vertex_attrib.push_back(normals[normal_index + 1]);
		vertex_attrib.push_back(normals[normal_index + 2]);
	}
	return vertex_attrib;
}

int main()
{
	// glfw: initialize and configure
	// ------------------------------
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // uncomment this statement to fix compilation on OS X
#endif

														 // glfw window creation
														 // --------------------
	GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "assignment0", NULL, NULL);
	if (window == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
	glfwSetCursorPosCallback(window, mouse_callback);
	glfwSetScrollCallback(window, scroll_callback);
	// glad: load all OpenGL function pointers
	// ---------------------------------------
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}

	float current_time = glfwGetTime();
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);
	Shader my_shader("assignment0.vs", "assignment0.fs");


	std::vector<float> vertices;
	std::vector<float> normals;
	std::vector<unsigned int> vertex_indices;
	std::vector<unsigned int> normal_indices;

	if (!parse_obj("torus.obj", vertices, normals, vertex_indices, normal_indices)) {
		std::cout << "Failed to parse file!!!!!!!!!!!!!" << std::endl;
		return -1;
	}
	int vertex_num = vertex_indices.size();

	unsigned int vao;
	glGenVertexArrays(1, &vao);

	unsigned int vbo;
	glGenBuffers(1, &vbo);

	glBindVertexArray(vao);

	std::vector<float> vertex_attribs = generate_vertex_attrib(vertices, normals, vertex_indices, normal_indices);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, vertex_attribs.size() * sizeof(float), &vertex_attribs[0], GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
	glBindVertexArray(0);

	glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
	// render loop
	// -----------
	my_shader.use();
	while (!glfwWindowShouldClose(window))
	{
		// input
		// -----
		processInput(window);
		float last_time = current_time;
		current_time = glfwGetTime();
		delta_time = current_time - last_time;
		// render
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		if (is_rotate) {
			rotate_radian += delta_time * glm::pi<float>() / 30;
		}
		float x = cos(rotate_radian), y = sin(rotate_radian);
		glm::mat4 model = glm::mat4(
			x, 0, -y, 0,
			0, 1, 0, 0,
			y, 0, x, 0,
			0, 0, 0, 1);
		my_shader.set_mat4("model", model);
		glm::mat4 projection = glm::perspective(glm::radians(fov), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
		my_shader.set_mat4("projection", projection);
		glm::mat4 view = glm::lookAt(camera_pos, camera_target, camera_up);
		my_shader.set_mat4("view", view);
		my_shader.set_vec3("light_pos", light_pos);
		glm::vec3 color;
		if (is_change_color) {
			int origin_index = model_color_index - 1;
			if (origin_index < 0)
				origin_index = 2;
			float ratio = (current_time - change_color_time) / 2.0;
			if (ratio > 1.0f)
			{
				ratio = 1.0f;
				is_change_color = false;
			}
			color = (1.0f - ratio) * model_color_array[origin_index] + ratio * model_color_array[model_color_index];
		}
		else
			color = model_color_array[model_color_index];
		my_shader.set_vec3("model_color", color);
		glBindVertexArray(vao);
		glDrawArrays(GL_TRIANGLES, 0, vertex_num);
		glBindVertexArray(0);
		// glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
		// -------------------------------------------------------------------------------
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	// glfw: terminate, clearing all previously allocated GLFW resources.
	// ------------------------------------------------------------------
	glDeleteVertexArrays(1, &vao);
	glDeleteBuffers(1, &vbo);
	glfwTerminate();
	return 0;
}

// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
// ---------------------------------------------------------------------------------------------------------
void processInput(GLFWwindow *window)
{
	static bool left_arrow_pressed = false;
	static bool right_arrow_pressed = false;
	static bool up_arrow_pressed = false;
	static bool down_arrow_pressed = false;
	static bool c_pressed = false;
	static bool r_pressed = false;

	if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS)
	{
		if (!left_arrow_pressed)
			light_pos.x -= 0.5f;
		left_arrow_pressed = true;
	}
	else if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_RELEASE)
		left_arrow_pressed = false;

	if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS)
	{
		if (!right_arrow_pressed)
			light_pos.x += 0.5f;
		right_arrow_pressed = true;
	}
	else if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_RELEASE)
		right_arrow_pressed = false;

	if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)
	{
		if (!up_arrow_pressed)
			light_pos.y += 0.5f;
		up_arrow_pressed = true;
	}
	else if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_RELEASE)
		up_arrow_pressed = false;

	if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
	{
		if (!down_arrow_pressed)
			light_pos.y -= 0.5f;
		down_arrow_pressed = true;
	}
	else if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_RELEASE)
		down_arrow_pressed = false;

	if (glfwGetKey(window, GLFW_KEY_C) == GLFW_PRESS) {
		if (!c_pressed)
		{
			if (!is_change_color) {
				model_color_index += 1;
				if (model_color_index > 2)
					model_color_index = 0;
				is_change_color = true;
				change_color_time = glfwGetTime();
			}		
		}
		c_pressed = true;
	}
	else if (glfwGetKey(window, GLFW_KEY_C) == GLFW_RELEASE)
		c_pressed = false;

	if (glfwGetKey(window, GLFW_KEY_R) == GLFW_PRESS) {
		if (!r_pressed) {
			is_rotate = !is_rotate;
		}
		r_pressed = true;
	}
	else if (glfwGetKey(window, GLFW_KEY_R) == GLFW_RELEASE)
		r_pressed = false;
}

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	// make sure the viewport matches the new window dimensions; note that width and 
	// height will be significantly larger than specified on retina displays.
	SCR_WIDTH = width;
	SCR_HEIGHT = height;
	glViewport(0, 0, width, height);
}

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset) {
	if (fov >= 1.0f && fov <= 45.0f)
		fov -= yoffset;
	if (fov <= 1.0f)
		fov = 1.0f;
	if (fov >= 45.0f)
		fov = 45.0f;
}

void mouse_callback(GLFWwindow* window, double xpos, double ypos) {
	static float yaw = 90.0f;
	static float pitch = 0.0f;
	static bool is_first_mouse = true;
	static double last_xpos, last_ypos;
	if (is_first_mouse) {
		is_first_mouse = false;
		last_xpos = xpos;
		last_ypos = ypos;
	}
	float x_offset = xpos - last_xpos;
	float y_offset = ypos - last_ypos;
	last_xpos = xpos;
	last_ypos = ypos;
	yaw -= x_offset * 0.1f;
	pitch -= y_offset * 0.1f;
	if (pitch > 75.0f)
		pitch = 75.0f;
	else if (pitch < -75.0f)
		pitch = -75.0f;
	camera_pos.y = 5.0f * sin(glm::radians(pitch));
	camera_pos.x = 5.0f * cos(glm::radians(pitch)) * cos(glm::radians(yaw));
	camera_pos.z = 5.0f * cos(glm::radians(pitch)) * sin(glm::radians(yaw));
}