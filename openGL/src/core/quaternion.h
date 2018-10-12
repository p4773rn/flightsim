#ifndef A_QUATERNION
#define A_QUATERNION
#include <glm/glm.hpp>
#include <math.h>
#include <iostream>

namespace core {
  class Quaternion {
  public:
    Quaternion();
    Quaternion(float _real, glm::vec3 _img);
    Quaternion(const Quaternion& q);
    void        operator=(const Quaternion& q);
    Quaternion  operator+(const Quaternion& q) const;
    Quaternion  operator-(const Quaternion& q) const;
    Quaternion  operator*(const Quaternion& q) const;
    Quaternion  operator*(const float c) const;
    float       dot(const Quaternion& q) const;
    Quaternion  conj() const;
    Quaternion  inverse() const;
    float       norm() const;
    Quaternion  normalized() const;
    Quaternion& normalize();
    Quaternion  slerp(const Quaternion& q, float t) const;
    friend std::ostream& operator<<(std::ostream& out, const Quaternion& q);
    float real;
    glm::vec3 img;
  };

  inline Quaternion Quaternion::operator+(const Quaternion& q) const{
    return Quaternion(real + q.real, img + q.img);
  }
  inline Quaternion Quaternion::operator-(const Quaternion& q) const{
    return Quaternion(real - q.real, img - q.img);
  }
  inline Quaternion Quaternion::operator*(const Quaternion& q) const{
    return Quaternion(real*q.real - glm::dot(img, q.img),
                      q.img * real + img * q.real
                      + glm::cross(img, q.img));
  }
  inline Quaternion Quaternion::operator*(const float c) const{
    return Quaternion(real * c, img * c);
  }
  inline Quaternion Quaternion::conj() const{
    return Quaternion(real, -img);
  }
  inline float Quaternion::norm() const{
    return sqrt(real*real + glm::dot(img, img));
  }
  inline Quaternion Quaternion::normalized() const{
    return Quaternion(real/norm(), img/norm());
  }
  inline Quaternion Quaternion::inverse() const{
    Quaternion inv = conj();
    float norm_sqr = real*real + glm::dot(img, img);
    inv.real /= norm_sqr;
    inv.img /= norm_sqr;
    return inv;
  }
  inline float Quaternion::dot(const Quaternion& q) const{
    return real * q.real + glm::dot(img, q.img);
  }
}

#endif