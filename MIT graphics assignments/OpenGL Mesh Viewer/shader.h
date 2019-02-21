#ifndef SHADER_H
#define SHADER_H

#include  <glad/glad.h>
#include  <string>
#include  <fstream>
#include  <sstream>
#include  <iostream>

class Shader {
public:
	unsigned int ID;

	// construtor generates the shader on the fly.
	Shader(const char *vertex_path, const char *fragment_path) {
		std::string vertex_code;
		std::string fragment_code;
		std::ifstream v_file;
		std::ifstream f_file;

		v_file.exceptions(std::ifstream::failbit | std::ifstream::badbit);
		f_file.exceptions(std::ifstream::failbit | std::ifstream::badbit);
		try {
			v_file.open(vertex_path);
			f_file.open(fragment_path);
			std::stringstream v_stream, f_stream;
			v_stream << v_file.rdbuf();
			f_stream << f_file.rdbuf();
			vertex_code = v_stream.str();
			fragment_code = f_stream.str();
		}
		catch (std::ifstream::failure e) {
			std::cout << "ERROR::SHADER::FILE_NOT_SUCCESSFULLY_READ" << std::endl;
		}

		const char *v_code = vertex_code.c_str();
		const char *f_code = fragment_code.c_str();
		unsigned int vertex, fragment;

		vertex = glCreateShader(GL_VERTEX_SHADER);
		glShaderSource(vertex, 1, &v_code, NULL);
		glCompileShader(vertex);
		check_compile_errors(vertex, "VERTEX");

		fragment = glCreateShader(GL_FRAGMENT_SHADER);
		glShaderSource(fragment, 1, &f_code, NULL);
		glCompileShader(fragment);
		check_compile_errors(fragment, "FRAGMENT");

		ID = glCreateProgram();
		glAttachShader(ID, vertex);
		glAttachShader(ID, fragment);
		glLinkProgram(ID);
		check_compile_errors(ID, "PROGRAM");

		glDeleteShader(vertex);
		glDeleteShader(fragment);

	}

	void use() {
		glUseProgram(ID);
	}

	void set_mat4(const char *name, const glm::mat4 &mat) const {
		glUniformMatrix4fv(glGetUniformLocation(ID, name), 1, GL_FALSE, &mat[0][0]);
	}

	void set_mat2(const char *name, const glm::mat2 &mat) const {
		glUniformMatrix2fv(glGetUniformLocation(ID, name), 1, GL_FALSE, &mat[0][0]);
	}

	void set_mat3(const char *name, const glm::mat3 &mat) const {
		glUniformMatrix3fv(glGetUniformLocation(ID, name), 1, GL_FALSE, &mat[0][0]);
	}

	void set_vec2(const char *name, const glm::vec2 &value) const {
		glUniform2fv(glGetUniformLocation(ID, name), 1, &value[0]);
	}

	void set_vec3(const char *name, const glm::vec3 &value) const {
		glUniform3fv(glGetUniformLocation(ID, name), 1, &value[0]);
	}

	void set_vec4(const char *name, const glm::vec4 &value) const {
		glUniform4fv(glGetUniformLocation(ID, name), 1, &value[0]);
	}

private:
	void check_compile_errors(unsigned int shader, std::string type) {
		int success;
		char info_log[1024];
		if (type != "PROGRAM")
		{
			glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
			if (!success) {
				glGetShaderInfoLog(shader, 1024, NULL, info_log);
				std::cout << "ERROR::SHADER_COMPILATION_ERROR of type: " << type << "\n" << info_log << "\n ------------------------------------------" << std::endl;
			}
		}
		else
		{
			glGetProgramiv(shader, GL_LINK_STATUS, &success);
			if (!success) {
				glGetProgramInfoLog(shader, 1024, NULL, info_log);
				std::cout << "ERROR::SHADER_LINKING_ERROR of type: " << type << "\n" << info_log << "\n ----------------------------------------------" << std::endl;
			}
		}
	}
};

#endif