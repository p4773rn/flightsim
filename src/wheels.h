#ifndef WHEELS_H
#define WHEELS_H

#include "vec2.h"
#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>
#include <tuple>
#include <vector>

class Wheels {
public:
    Wheels(double stiffness, double frictionCoefficient, double springFriction, const glm::dvec3& position) :
        stiffness{stiffness},
        frictionCoefficient{frictionCoefficient},
        springFriction{springFriction},
        position{position},
        brakesOn{false}
    {}

    std::tuple<glm::dvec3, glm::dvec3> getForceAndTorque(
        const glm::dvec3& velocity, 
        const glm::dvec3& planePosition, 
        const glm::dvec3 angularVelocity, 
        const glm::dquat& orientation, 
        const glm::dvec3& netForce,
        std::vector<std::tuple<glm::vec3, glm::vec3, glm::vec3>>& debugArrows) const;

    void toggleBrakes() {brakesOn = !brakesOn;};
    bool getBrakesStatus() const {return brakesOn;}

private:
    double stiffness; // stiffness of amortization spring
    double frictionCoefficient; // friction of wheels
    double springFriction; // TEST?
    glm::dvec3 position; // position (where it is attached) relative to the plane
    bool brakesOn;
};

#endif
