
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "camera_manager.h"

CameraManager& CameraManager::instance() {
	static CameraManager camera_manager;
	return camera_manager;
}

void CameraManager::regenerate_camera_matrix() {
	camera_matrix = glm::lookAt(camera_pos, camera_target, camera_up);
}

void CameraManager::regenerate_projection_matrix() {
	projection_matrix = glm::perspective(fov, aspect_ratio, near_plane, far_plane);
}

void CameraManager::set_camera_argument(const glm::vec3& camera_pos, const glm::vec3& camera_target, const glm::vec3& camera_up) {
	this->camera_pos = camera_pos;
	this->camera_target = camera_target;
	this->camera_up = camera_up;
	regenerate_camera_matrix();
}

void CameraManager::set_projection_argument(float fov, float aspect_ratio, float near_plane, float far_plane) {
	this->fov = fov;
	this->aspect_ratio = aspect_ratio;
	this->near_plane = near_plane;
	this->far_plane = far_plane;
	regenerate_projection_matrix();
}
