#ifndef WHEELS_H
#define WHEELS_H

#include "vec2.h"
#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>
#include <tuple>

class Wheels {
public:
    Wheels(double stiffness, double frictionCoefficient, double springFriction, const Vec2& position) :
        stiffness{stiffness},
        frictionCoefficient{frictionCoefficient},
        springFriction{springFriction},
        position{position},
        brakesOn{false}
    {}

    std::tuple<glm::dvec3, glm::dvec3> getForceAndTorque(const glm::dvec3& velocity, const glm::vec3& planePosition, const glm::dvec3 angularVelocity, const glm::dquat& orientation , double mass) const;

    void toggleBrakes() {brakesOn = !brakesOn;};
    bool getBrakesStatus() const {return brakesOn;}

private:
    double stiffness; // stiffness of amortization spring
    double frictionCoefficient; // friction of wheels
    double springFriction; // TEST?
    Vec2 position; // position realtive to the plane
    bool brakesOn;
};

#endif
