#ifndef AZ_CAMERA
#define AZ_CAMERA

#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtc/matrix_transform.hpp>

class Camera {
public:
  Camera();
  Camera(const glm::vec3& _position);
  glm::mat4 get_view();
  glm::mat4 get_view_no_translate();
  glm::vec3 get_position();
  void move_mouse(float x_offset, float y_offset);
  void move_position(unsigned int dir);
  void set_position(const glm::vec3& _position) { position = _position;};
  void look_in(const glm::vec3& _direction);
  void orbit(float x_offset, float y_offset, float distance, const glm::vec3& center);
  void set_right(const glm::vec3& right) {this->right = right;}
private:
  glm::vec3 position;
  glm::vec3 direction;
  glm::vec3 right;
  
  float pitch;
  float yaw;

  // for orbiting
  float phi = 0; 
  float theta = 3.14/2;
};

#endif
