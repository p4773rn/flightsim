#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "camera.h"

Camera::Camera() : 
		pitch{0}, yaw{0}
{}

Camera::Camera(const glm::vec3& _position) : 
		pitch{0}, yaw{0}, position{_position}
{}

void Camera::moveMouse(float x_offset, float y_offset) {
	//if (pitch + y_offset < 90.0f && pitch + y_offset > 0)
	pitch += 90 * y_offset;
	yaw -= 90 * x_offset;
	if (yaw > 360) {
		yaw -= 360;
	}
	if (yaw < 0) {
		yaw += 360;
	}
	float siny = sin(glm::radians(yaw));
	float sinp = sin(glm::radians(pitch));
	float cosy = cos(glm::radians(yaw));
	float cosp = cos(glm::radians(pitch));
	direction.x = siny*cosp;
	direction.y = -sinp;
	direction.z = cosy*cosp;
	right.x = cosy;
	right.y = 0;
	right.z = -siny;
}


void Camera::movePosition(unsigned int dir) {
	const int m = 20;
	switch(dir) {
	case 0:
		position -= right * (m * 0.06f);
		break;
	case 1:
		position += right * (m * 0.06f);
		break;
	case 2:
		position -= direction * (m * 0.06f);
		break;
	case 3:
		position += direction * (m * 0.06f);
		break;
	}
}

glm::mat4 Camera::getView() {
	glm::vec3 up = glm::cross(direction, right);
	double mat4[16] = {
		right.x, up.x, direction.x, 0,
        right.y, up.y, direction.y, 0,
        right.z, up.z, direction.z, 0,
              0,    0,           0, 1
    };
    glm::mat4 M = glm::mat4(1.0f);
	glm::mat4 C;
	M = glm::translate(M, -position);
	C = glm::make_mat4(mat4);
	return C * M;
}
