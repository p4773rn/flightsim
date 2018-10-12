#ifndef AZ_CAMERA
#define AZ_CAMERA

#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtc/matrix_transform.hpp>

class Camera {
public:
  Camera();
  Camera(const glm::vec3& _position);
  glm::mat4 getView();
  void moveMouse(float x_offset, float y_offset);
  void movePosition(unsigned int dir);
//private:
  glm::vec3 position;
  glm::vec3 direction;
  glm::vec3 right;
  float pitch;
  float yaw;
};

#endif