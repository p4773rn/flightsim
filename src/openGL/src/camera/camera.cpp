#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "camera.h"

Camera::Camera() : 
pitch{0}, yaw{0}
{}

Camera::Camera(const glm::vec3& _position) : pitch{0}, yaw{0}, position{_position}
{
    move_mouse(0, 0);   
}

void Camera::move_mouse(float x_offset, float y_offset) {
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


void Camera::move_position(unsigned int dir) {
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

glm::mat4 Camera::get_view() {
  glm::mat4 M = glm::mat4(1.0f);
  M = glm::translate(M, -position);
  return get_view_no_translate() * M;
}

glm::mat4 Camera::get_view_no_translate() {
  glm::vec3 up = glm::cross(direction, right);
  double mat4[16] = {
    right.x,  up.x, direction.x,  0,
    right.y,  up.y, direction.y,  0,
    right.z,  up.z, direction.z,  0,
          0,     0,           0,  1
  };
  return glm::make_mat4(mat4);
}

glm::vec3 Camera::get_position(){
  return position;
}
