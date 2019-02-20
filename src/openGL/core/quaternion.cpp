#include "quaternion.h"
#include <stdexcept>

namespace core {
  Quaternion::Quaternion() : real{0.0f}, img{glm::vec3(0.0f, 0.0f, 0.0f)} {}
  Quaternion::Quaternion(float _real, glm::vec3 _img) : real(_real), img(_img) {}
  Quaternion::Quaternion(const Quaternion& q) : real(q.real), img(q.img) {}

  void Quaternion::operator=(const Quaternion& q) {
    real = q.real;
    img = q.img;
  }

  Quaternion& Quaternion::normalize() {
    float n = norm();
    real /= n;
    img /= n;
    return *this;
  }

  Quaternion Quaternion::slerp(const Quaternion& q, float t) const{
    if (t > 1.0f && t < 0.0f) {
      throw std::invalid_argument("t must be between 0 and 1");
    }
    float theta = acos(this->dot(q));
    return *this *(sin(theta * (1.0f - t))/sin(theta)) + q * (sin(theta * t)/sin(theta));
  }

  std::ostream& operator<<(std::ostream& out, const Quaternion& q) {
    out << "(" << q.real << ", " 
              << q.img.x << ", "
              << q.img.y << ", "
              << q.img.z << ")";
    return out;
  }
}

