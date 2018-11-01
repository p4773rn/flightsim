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
        breaksOn{false}
    {}

    Vec2 getForce(const Vec2& velocity, const Vec2& planePosition, double angularVelocity, double angle) const;

    void toggleBreaks();
    bool getBreaksStatus() {return breaksOn;}

private:
    double stiffness; // stiffness of amortization spring
    double frictionCoefficient; // friction of wheels
    double springFriction; // TEST?
    Vec2 position; // position realtive to the plane
    bool breaksOn;
};

#endif
