#ifndef WHEELS_H
#define WHEELS_H

#include "vec2.h"
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

    std::tuple<Vec2, double> getForceAndTorque(const Vec2& velocity, const Vec2& planePosition, double angularVelocity, double angle, double mass) const;

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
