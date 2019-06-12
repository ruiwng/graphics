#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <iostream>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "box.h"
#include "shader.h"
#include "camera_manager.h"
#include "sky_box.h"
#include "sphere.h"

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void mouse_btn_callback(GLFWwindow* window, int button, int action, int mods);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void processInput(GLFWwindow* window);

// settings
const unsigned int SCR_WIDTH = 1200;
const unsigned int SCR_HEIGHT = 800;

// camera
glm::vec3 camera_pos = glm::vec3(0.0f, 0.0f, 3.0f);
glm::vec3 camera_front = glm::vec3(0.0f, 0.0f, -1.0f);
glm::vec3 camera_up = glm::vec3(0.0f, 1.0f, 0.0f);

bool left_mouse_press = false;

//timing
float delta_time = 0.0f;  // time between current frame and last frame
float last_frame = 0.0f;

float fov = 45.0f;

int main() {
	// glfw: initialize and configure
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // uncomment this statement to fix compilation on OS X
#endif

														 //glfw window creation
	GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "GraphicsShaders", NULL, NULL);
	if (window == NULL) {
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}

	glfwMakeContextCurrent(window);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
	glfwSetCursorPosCallback(window, mouse_callback);
	glfwSetScrollCallback(window, scroll_callback);
	glfwSetMouseButtonCallback(window, mouse_btn_callback);

	// glad: load all OpenGL function pointers
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
		std::cout << "Failed to initilize GLAD" << std::endl;
		return -1;
	}

	// build and compile our shader program
	// ====================================
	// vertex shader
	SkyBox* p_sky_box = new SkyBox("resources/nvposx.bmp", "resources/nvnegx.bmp", "resources/nvposy.bmp", 
		"resources/nvnegy.bmp",  "resources/nvposz.bmp", "resources/nvnegz.bmp");
	/*Shader* p_sphere_shader = new Shader("sphere.vert", "sphere.frag");
	Sphere* p_sphere = new Sphere(30, 30);*/
	/*SkyBox* p_sky_box = new SkyBox("resources/nvnegx.bmp", "resources/nvnegx.bmp", "resources/nvnegx.bmp",
		"resources/nvnegx.bmp", "resources/nvnegx.bmp", "resources/nvnegx.bmp");*/

	// uncomment this call to dray in wireframe polygons
	// glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);
	glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
	// render loop
	// -----------
	
	while (!glfwWindowShouldClose(window)) {
		//input
		processInput(window);

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		CameraManager::instance().set_camera_argument(camera_pos, camera_pos + camera_front, camera_up);
		CameraManager::instance().set_projection_argument(glm::radians(fov), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
		p_sky_box->draw();
		//p_sphere_shader->use();
		//glm::mat4 view = CameraManager::instance().get_camera_matrix();
		//glm::mat4 projection = CameraManager::instance().get_projection_matrix();
		//p_sphere_shader->setUniform("view", view);
		//p_sphere_shader->setUniform("projection", projection);
		//p_sphere->draw();
		// glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
		glfwSwapBuffers(window);
		glfwPollEvents();
	}
	delete p_sky_box;
	glfwTerminate();
	return 0;
}

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
	// make sure the viewport matches the new window dimensions; note that width and
	// height will be sigificantly larger than specified on retina displays
	glViewport(0, 0, width, height);
}

// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
// ---------------------------------------------------------------------------------------------------------
void processInput(GLFWwindow* window) {
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);
	float cameraSpeed = 2.5 * delta_time;
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		camera_pos += camera_front * cameraSpeed;
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		camera_pos -= camera_front * cameraSpeed;
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		camera_pos -= glm::normalize(glm::cross(camera_front, camera_up)) * cameraSpeed;
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		camera_pos += glm::normalize(glm::cross(camera_front, camera_up)) * cameraSpeed;
}

void mouse_callback(GLFWwindow* window, double xpos, double ypos) {
	static double last_x;
	static double last_y;
	static double yaw = -90.0f;
	static double pitch = 0.0f;
	static bool firstMouse = true;
	if (!left_mouse_press)
	{
		firstMouse = true;
		return;
	}
	if (firstMouse) {
		last_x = xpos;
		last_y = ypos;
		firstMouse = false;
		return;
	}
	double deltaX = xpos - last_x;
	double deltaY = last_y - ypos;
	last_x = xpos;
	last_y = ypos;

	double sensitivity = 0.1f;
	yaw += deltaX * sensitivity;
	pitch += deltaY * sensitivity;
	if (pitch > 89.0f)
		pitch = 89.0f;
	else if (pitch < -89.0f)
		pitch = -89.0f;

	camera_front.y = sin(glm::radians(pitch));
	camera_front.x = cos(glm::radians(pitch)) * cos(glm::radians(yaw));
	camera_front.z = cos(glm::radians(pitch)) * sin(glm::radians(yaw));
}

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset) {
	fov -= yoffset;
	if (fov <= 1.0f)
		fov = 1.0f;
	if (fov >= 80.0f)
		fov = 80.0f;
}

void mouse_btn_callback(GLFWwindow* window, int button, int action, int mods) {
	if (button != GLFW_MOUSE_BUTTON_LEFT)
		return;
	if (action == GLFW_PRESS)
		left_mouse_press = true;
	else if (action == GLFW_RELEASE)
		left_mouse_press = false;
}