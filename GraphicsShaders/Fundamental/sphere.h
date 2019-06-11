#ifndef  SPHERE_H
#define  SPHERE_H

#include <vector>

class Sphere {
public:
	Sphere(int horizontal_vertex_count, int vertical_vertex_count): horizontal_vertex_count(horizontal_vertex_count),
	vertical_vertex_count(vertical_vertex_count), VAO(0), VBO(0), EBO(0){
		//init_buffer();
	}

	~Sphere();
	void draw();


private:
	void init_buffer();
	int horizontal_vertex_count, vertical_vertex_count;
	std::vector<float> vbo_buffer;
	std::vector<unsigned int> ebo_buffer;
	unsigned int VAO;   // vertex array object
	unsigned int VBO, EBO;
	int element_count;
};

#endif
