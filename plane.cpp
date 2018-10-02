#include "plane.h"
#include "environment.h"

#include <iostream>

void Plane::update(double delta) {
    // Forces update

    Vec2 netForce;
    
    // Weight
    netForce += Vec2(0, -GRAVITATIONAL_ACCELERATION * mass);
    
    // Lift
    netForce += wing.getLift(velocity, angle, getAirPressure(pos.getY()));
    // std::cout << "Net X: " << netForce.getX() << std::endl;
    // std::cout << "Net Y: " << netForce.getY() << std::endl << std::endl;

    // Drag
    // TODO: get Drag coef table and calculate drag force in airfoil.cpp

    // Thrust
    netForce += engine.getThrust(angle);

    velocity += netForce / mass * delta;
    pos += velocity * delta;

    // Torque update
    // TODO: get MOMENT COEFFICIENT and calculate angular acceleration

}
