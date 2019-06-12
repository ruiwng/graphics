#ifndef CAMERA_MANAGER_H
#define CAMERA_MANAGER_H

#include <glm/glm.hpp>

class CameraManager {
private:
	CameraManager() {}
	glm::mat4 camera_matrix;
	glm::mat4 projection_matrix;

	// camera related
	glm::vec3 camera_pos;
	glm::vec3 camera_target;
	glm::vec3 camera_up;

	// projection related
	float fov; 
	float aspect_ratio;
	float near_plane;
	float far_plane;

public:
	glm::mat4 get_camera_matrix() const { return camera_matrix; }
	glm::mat4 get_projection_matrix() const { return projection_matrix; }
	void set_camera_argument(const glm::vec3& camera_pos, const glm::vec3& camera_target, const glm::vec3& camera_up);
	void set_projection_argument(float fov, float aspect_ratio, float near_plane, float far_plane);
	glm::vec3 get_camera_pos() const { return camera_pos; }
	static CameraManager& instance();

private:
	void regenerate_camera_matrix();
	void regenerate_projection_matrix();
};

#endif
