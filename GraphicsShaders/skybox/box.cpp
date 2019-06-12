#include <glad/glad.h>

#include "box.h"

void Box::draw() {
	if (vbo_buffer.empty())
		init_buffer();
	glBindVertexArray(VAO);
	glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);
}

void Box::init_buffer() {
	
	float vertices[] = {
		-0.5f, -0.5f, -0.5f,
		 0.5f, -0.5f, -0.5f,
		 0.5f,  0.5f, -0.5f,
		-0.5f,  0.5f, -0.5f,

		-0.5f, -0.5f, 0.5f,
		0.5f, -0.5f,  0.5f,
		0.5f,  0.5f,  0.5f,
		-0.5f,  0.5f, 0.5f,
	};

	unsigned int indices[] = {
		0, 3, 7, // -x
		7, 4, 0,

		1, 2, 6, // +x
		6, 5, 1,

		1, 0, 4, // -y
		4, 5, 1,

		2, 3, 7, // +y
		7, 6, 2,

		0, 1, 2, // -z
		2, 3, 0,

		4, 5, 6, // +z
		6, 7, 4,
	};

	vbo_buffer = std::vector<float>(vertices, vertices + sizeof(vertices));
	ebo_buffer = std::vector<unsigned int>(indices, indices + sizeof(indices));

	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);
	// bind the Vertex Array Object first, then bind and set vertex buffer(s), and then configure vertex attribute(s)
	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, vbo_buffer.size(), &vbo_buffer[0], GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, ebo_buffer.size(), &ebo_buffer[0], GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
}

Box::~Box() {
	if (VAO != 0) {
		glDeleteVertexArrays(1, &VAO);
		VAO = 0;
	}
	if (VBO != 0) {
		glDeleteBuffers(1, &VBO);
		VBO = 0;
	}
	if (EBO != 0) {
		glDeleteBuffers(1, &EBO);
		EBO = 0;
	}
}