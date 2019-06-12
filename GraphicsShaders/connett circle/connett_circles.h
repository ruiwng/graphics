#ifndef CONNETT_CIRCLES
#define CONNETT_CIRCLES

#include <vector>
#include "shader.h"

class ConnettCircles {
public:
	ConnettCircles(float u_side): VAO(0), VBO(0), EBO(0), p_shader(nullptr), u_side(u_side) {
		//init_buffer();
	}

	~ConnettCircles();
	void draw();


private:
	void init();
	Shader* p_shader;
	std::vector<float> vbo_buffer;
	std::vector<unsigned int> ebo_buffer;
	unsigned int VAO;   // vertex array object
	unsigned int VBO, EBO;
	int element_count;
	float u_side;
};

#endif
