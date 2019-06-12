#ifndef BOX_H
#define BOX_H

#include <vector>

class Box {
public:
	Box(): VAO(0), VBO(0), EBO(0) {
		//init_buffer();
	}

	~Box();
	void draw();


private:
	void init_buffer();
	std::vector<float> vbo_buffer;
	std::vector<unsigned int> ebo_buffer;
	unsigned int VAO;   // vertex array object
	unsigned int VBO, EBO;
	int element_count;
};

#endif

