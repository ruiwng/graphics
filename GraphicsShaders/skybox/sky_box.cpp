#include <glad/glad.h>
#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "sky_box.h"
#include "camera_manager.h"

void SkyBox::draw() {
	if (p_box == nullptr)
		init();
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_CUBE_MAP, cube_texture);

	p_shader->use();
	p_shader->setUniform("skyBox", 0);
	glm::mat4 view = CameraManager::instance().get_camera_matrix();
	glm::mat4 projection = CameraManager::instance().get_projection_matrix();
	glm::vec3 camera_pos = CameraManager::instance().get_camera_pos();
	glm::mat4 model(1.0f);
	model = glm::translate(model, camera_pos);
	p_shader->setUniform("model", model);
	p_shader->setUniform("view", view);
	glm::mat4 temp = view * model;
	p_shader->setUniform("projection", projection);

	p_box->draw();
}

void SkyBox::init() {
	p_box = new Box;
	p_shader = new Shader("sky_box.vert", "sky_box.frag");

	glActiveTexture(GL_TEXTURE0);
	glGenTextures(1, &cube_texture);
	
	glBindTexture(GL_TEXTURE_CUBE_MAP, cube_texture);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

	for (int i = 0; i < 6; ++i) {
		int height, width, nrChannels;
		unsigned char* data = stbi_load(image_list[i].c_str(), &width, &height, &nrChannels, 0);
		if (data) {
			glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X_EXT + i, 0, GL_RGB8, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
		}
	}
}

SkyBox::~SkyBox() {
	if (p_box != nullptr)
	{
		delete p_box;
		p_box = nullptr;
	}
	if (p_shader != nullptr) {
		delete p_shader;
		p_shader = nullptr;
	}

	if (cube_texture != 0) {
		glDeleteTextures(1, &cube_texture);
	}
}