#include <glad/glad.h>

#include "quad.h"

void Quad::draw() {
	if (vbo_buffer.empty())
		init_buffer();
	glBindVertexArray(VAO);
	glDrawElements(GL_TRIANGLES, element_count, GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);
}

void Quad::init_buffer() {
	int vertex_count = horizontal_vertex_count * vertical_vertex_count;
	vbo_buffer.reserve(vertex_count * 3);
	float horizontal_step = 2.0f / (horizontal_vertex_count - 1);
	float vertical_step = 2.0f / (vertical_vertex_count - 1);

	for (int i = 0; i < horizontal_vertex_count; ++i) {
		float x_pos = -1 + horizontal_step * i;
		for (int j = 0; j < vertical_vertex_count; ++j) {
			float pitch = vertical_step * j;
			vbo_buffer.push_back(x_pos);
			vbo_buffer.push_back(-1 + vertical_step * j);
		}
	}

	element_count = (horizontal_vertex_count - 1) * (vertical_vertex_count - 1) * 6;
	ebo_buffer.reserve(element_count);
	for (unsigned int i = 0; i < horizontal_vertex_count - 1; ++i) {
		for (unsigned int j = 0; j < vertical_vertex_count - 1; ++j) {
			// lower-left triangle
			ebo_buffer.push_back(i * vertical_vertex_count + j);
			ebo_buffer.push_back((i + 1) * vertical_vertex_count + j);
			ebo_buffer.push_back(i * vertical_vertex_count + j + 1);
			

			// upper-right triangle
			ebo_buffer.push_back(i * vertical_vertex_count + j + 1);
			ebo_buffer.push_back((i + 1) * vertical_vertex_count + j);
			ebo_buffer.push_back((i + 1) * vertical_vertex_count + j + 1);
			
		}
	}

	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);
	// bind the Vertex Array Object first, then bind and set vertex buffer(s), and then configure vertex attribute(s)
	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, vertex_count * 2 * sizeof(float), &vbo_buffer[0], GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, element_count * sizeof(unsigned int), &ebo_buffer[0], GL_STATIC_DRAW);

	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
}

Quad::~Quad() {
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