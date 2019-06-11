#ifndef SHADER_H
#define SHADER_H
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

#include <glad/glad.h>
#include <assert.h>
#include <vector>

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>


class Shader {
public:
	Shader(const char* path1, const char* path2, const char* path3=nullptr, const char* path4=nullptr, const char* path5=nullptr) {
		const char *path_array[] = {path1, path2, path3, path4, path5};
		std::string code;
		std::ifstream file;
		file.exceptions(std::ifstream::failbit | std::ifstream::badbit);
		std::vector<unsigned int> shader_array;

		for (int i = 0; i < 5; ++i) {		
			try {
				// open file
				const char *path = path_array[i];
				if (path == nullptr)
					break;
				file.open(path);
				std::stringstream shader_stream;
				shader_stream << file.rdbuf();
				code = shader_stream.str();
				file.close();

				int length = strlen(path);
				unsigned int shader;
				if (length > 5 && strcmp(path + length - 5, ".vert") == 0)
					shader = glCreateShader(GL_VERTEX_SHADER);
				else if (length > 5 && strcmp(path + length - 5, ".frag") == 0)
					shader = glCreateShader(GL_FRAGMENT_SHADER);
				else if (length > 5 && strcmp(path + length - 5, ".geom") == 0)
					shader = glCreateShader(GL_GEOMETRY_SHADER);
				else if (length > 4 && strcmp(path + length - 4, ".tcs") == 0)
					shader = glCreateShader(GL_TESS_CONTROL_SHADER);
				else if (length > 4 && strcmp(path + length - 4, ".tvs") == 0)
					shader = glCreateShader(GL_TESS_EVALUATION_SHADER);
				else
				{
					std::cout << "unknown shader type " << path << std::endl;
					assert(false);
				}
				const char *code_address = code.c_str();
				glShaderSource(shader, 1, &code_address, NULL);
				glCompileShader(shader);
				checkCompileErrors(shader, path);
				shader_array.push_back(shader);
			}
			catch(std::ifstream::failure e){
				std::cout << "ERROR::SHADER::FILE_NOT_SUCCESSFULLY_READ" << std::endl;
			}
		}
		ID = glCreateProgram();
		for (unsigned int &shader : shader_array)
			glAttachShader(ID, shader);
		glLinkProgram(ID);
		checkLinkErrors(ID);

		for (unsigned int &shader : shader_array)
			glDeleteShader(shader);
		shader_array.clear();
	}

	// activate the shader
	void use() {
		glUseProgram(ID);
	}

	void setUniform(const char* name, bool value) const {
		glUniform1i(glGetUniformLocation(ID, name), int(value));
	}

	void setUniform(const char* name, int value) const {
		glUniform1i(glGetUniformLocation(ID, name), value);
	}

	void setUniform(const char* name, float value) const {
		glUniform1f(glGetUniformLocation(ID, name), value);
	}

	void setUniform(const char* name, glm::vec2& value) const {
		glUniform2fv(glGetUniformLocation(ID, name), 1, glm::value_ptr(value));
	}

	void setUniform(const char* name, glm::vec3& value) const {
		glUniform3fv(glGetUniformLocation(ID, name), 1, glm::value_ptr(value));
	}

	void setUniform(const char* name, glm::vec4& value) const {
		glUniform4fv(glGetUniformLocation(ID, name), 1, glm::value_ptr(value));
	}

	void setUniform(const char* name, glm::mat2& value) const {
		glUniformMatrix2fv(glGetUniformLocation(ID, name), 1, GL_FALSE, glm::value_ptr(value));
	}

	void setUniform(const char* name, glm::mat3& value) const {
		glUniformMatrix3fv(glGetUniformLocation(ID, name), 1, GL_FALSE, glm::value_ptr(value));
	}

	void setUniform(const char* name, glm::mat4& value) const {
		glUniformMatrix4fv(glGetUniformLocation(ID, name), 1, GL_FALSE, glm::value_ptr(value));
	}
private:
	static void checkCompileErrors(unsigned int shader, const char* name) {
		int success;
		char infoLog[1024];
		glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
		if (!success) {
			glGetShaderInfoLog(shader, 1024, NULL, infoLog);
			std::cout << "ERROR::SHADER_COMPILATION_ERROR " << name << "\n" << infoLog << "\n -----------------------------" << std::endl;
		}
	}

	static void checkLinkErrors(unsigned int program){
		int success;
		char infoLog[1204];
		glGetProgramiv(program, GL_LINK_STATUS, &success);
		if (!success) {
			glGetProgramInfoLog(program, 1024, NULL, infoLog);
			std::cout << "ERROR::PROGRAM_LINKING_ERROR \n" << infoLog << "\n -----------------------------------------" << std::endl;
		}
	}
	unsigned int ID;
};

#endif
