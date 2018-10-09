#include "plane.h" 
#include "environment.h"
#include <iostream>
#include <tuple>

void Plane::update(double delta) {
    double airDensity = getAirDensity(pos.getY());
    
    Vec2 wingsForce, elevatorsForce;
    double wingsTorque, elevatorsTorque;
    
    std::tie(wingsForce, wingsTorque) = wings.getForceAndTorque(velocity, angle, airDensity);

    Vec2 elevatorsVelocity = velocity; // TODO: elevator's velocity should depend on angularVelocity of the plane
    std::tie(elevatorsForce, elevatorsTorque) = elevators.getForceAndTorque(elevatorsVelocity, angle, airDensity);
    
    // Forces update
    Vec2 netForce;
    
    // Weight
    netForce += Vec2(0, -GRAVITATIONAL_ACCELERATION * mass);
    
    // Lift + Drag
    netForce += wingsForce;
    netForce += elevatorsForce;

    // Thrust
    netForce += engine.getThrust(angle);

    Vec2 acceleration = netForce / mass;
    velocity += acceleration * delta;
    pos += velocity * delta;

   
    // Torque update
    double torque = 0;
    
    torque += wingsTorque + wingsForce.getY() * wingsPoint;
    torque += elevatorsTorque + elevatorsForce.getY() * elevatorsPoint;


    double angularAcceleration = torque / inertia;
    angularVelocity += angularAcceleration * delta;   
    angle += angularVelocity * delta;
    
    // std::cout << "Lift: " << wings.getLiftMagnitude() << std::endl;
    // std::cout << "angVel: " << angularVelocity << std::endl << std::endl;
    

    //std::cout << "Velocity X: " << velocity.getX() << std::endl;
    //std::cout << "Velocity Y: " << velocity.getY() << std::endl << std::endl;
    //std::cout << "Net X: " << netForce.getX() << std::endl;
    //std::cout << "Net Y: " << netForce.getY() << std::endl;
    //std::cout << "Torque: " << torque << "; Angular velocity: " << angularVelocity << std::endl;
    //std::cout << "Angle: " << angle << std::endl;
    //std::cout << std::endl;
}

