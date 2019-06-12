#ifndef SKY_BOX_H
#define SKY_BOX_H

#include <vector>
#include <string>
#include "box.h"
#include "shader.h"

class SkyBox {
public:
	SkyBox(const char* pos_x, const char* neg_x, const char* pos_y, const char* neg_y, const char* pos_z, const char* neg_z):p_box(nullptr), p_shader(nullptr), cube_texture(0) {
		image_list.push_back(pos_x);
		image_list.push_back(neg_x);
		image_list.push_back(pos_y);
		image_list.push_back(neg_y);
		image_list.push_back(pos_z);
		image_list.push_back(neg_z);
	}
	~SkyBox();

	void draw();
	void init();

private:
	Box* p_box;
	Shader* p_shader;
	std::vector<std::string> image_list;
	unsigned int cube_texture;
};

#endif
