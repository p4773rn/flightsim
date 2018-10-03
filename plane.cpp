#include "plane.h"
#include "environment.h"

#include <iostream>

void Plane::update(double delta) {
    // Forces update

    Vec2 netForce;
    
    // Weight
    netForce += Vec2(0, -GRAVITATIONAL_ACCELERATION * mass);
    
    // Lift + Drag
    Vec2 wingsForce = wing.getForce(velocity, angle, getAirPressure(pos.getY()));
    netForce += wingsForce;
    // std::cout << "Net X: " << netForce.getX() << std::endl;
    // std::cout << "Net Y: " << netForce.getY() << std::endl << std::endl;

    // Thrust
    netForce += engine.getThrust(angle);

    velocity += netForce / mass * delta;
    pos += velocity * delta;

    // Torque update
    // TODO: get MOMENT COEFFICIENT and calculate angular acceleration
    angularVelocity = wing.getLiftMagnitude() / (mass * wingsPoint) * delta;
    // std::cout << "Lift: " << wing.getLiftMagnitude() << std::endl;
    // std::cout << "angVel: " << angularVelocity << std::endl << std::endl;
    angle += angularVelocity * delta;
}
