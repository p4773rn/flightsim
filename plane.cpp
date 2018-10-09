#include "plane.h" 
#include "environment.h"
#include <iostream>
#include <tuple>

using std::cout;
using std::endl;

void Plane::update(double delta) {
    double airDensity = getAirDensity(pos.getY());
    
    Vec2 wingsForce, elevatorsForce;
    double wingsTorque, elevatorsTorque;
    
    std::tie(wingsForce, wingsTorque) = wings.getForceAndTorque(velocity, angle, airDensity);

    Vec2 elevatorsVelocity = {velocity.getX() - angularVelocity * (elevatorsPoint * sin(angle)),
                              velocity.getY() + angularVelocity * (elevatorsPoint * cos(angle))};
    
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
    
    cout << "Pos: " << pos << endl;
    cout << "Vel: " << velocity << endl;
    cout << "Angle: " << angle / M_PI * 180 << " deg" << endl;
    cout << "AngVel: " << angularVelocity / M_PI * 180 << " deg/sec" << endl;
    cout << "Force: " << netForce << endl;
    cout << "Torque: " << torque << endl;
    cout << "Wings AoA: " << wings.getAngleOfAttack(velocity, angle) / M_PI * 180 << " deg" << endl;
    cout << "Elevators AoA: " << elevators.getAngleOfAttack(velocity, angle) / M_PI * 180 << " deg"  << endl;
    cout << endl;
}



Plane Plane::getDefaultPlane() {
    Airfoil wings(
            Table(
            {
                { -8.0 * M_PI / 180, -0.45, 0.022, 0.246, -0.097 },
                { -6.0 * M_PI / 180, -0.23, 0.014, 0.246, -0.092 },
                { -4.0 * M_PI / 180, -0.03, 0.012, 0.246, -0.092 },
                { -2.0 * M_PI / 180,  0.20, 0.010, 0.246, -0.092 },
                {  0.0 * M_PI / 180,  0.38, 0.010, 0.246, -0.093 },
                {  2.0 * M_PI / 180,  0.60, 0.010, 0.246, -0.095 },
                {  4.0 * M_PI / 180,  0.80, 0.012, 0.246, -0.098 },
                {  6.0 * M_PI / 180,  1.00, 0.014, 0.246, -0.100 },
                {  8.0 * M_PI / 180,  1.15, 0.017, 0.246, -0.100 },
                { 10.0 * M_PI / 180,  1.27, 0.022, 0.246, -0.095 },
                { 12.0 * M_PI / 180,  1.36, 0.030, 0.246, -0.092 },
                { 14.0 * M_PI / 180,  1.35, 0.042, 0.246, -0.092 },
                { 16.0 * M_PI / 180,  1.25, 0.059, 0.246, -0.095 }
            }),
            103, // area
            0.0523598775, // angle
            3.6 // chordLength
    );

    Airfoil elevators(
            Table(
            {
                { -14.0 * M_PI / 180, -0.87, 0.036, 0.25,  0.012 },
                { -12.0 * M_PI / 180, -0.89, 0.028, 0.25,  0.004 },
                { -10.0 * M_PI / 180, -0.90, 0.021, 0.25,  0.002 },
                {  -8.0 * M_PI / 180, -0.85, 0.018, 0.25,  0.0   },
                {  -6.0 * M_PI / 180, -0.64, 0.014, 0.25,  0.0   },
                {  -4.0 * M_PI / 180, -0.43, 0.011, 0.25,  0.0   },
                {  -2.0 * M_PI / 180, -0.21, 0.010, 0.25,  0.0   },
                {   0.0 * M_PI / 180,   0.0, 0.009, 0.25,  0.0   },
                {   2.0 * M_PI / 180,  0.21, 0.010, 0.25,  0.0   },
                {   4.0 * M_PI / 180,  0.43, 0.011, 0.25,  0.0   },
                {   6.0 * M_PI / 180,  0.64, 0.014, 0.25,  0.0   },
                {   8.0 * M_PI / 180,  0.85, 0.018, 0.25,  0.0   },
                {  10.0 * M_PI / 180,  0.90, 0.021, 0.25, -0.002 },
                {  12.0 * M_PI / 180,  0.89, 0.028, 0.25, -0.004 },
                {  14.0 * M_PI / 180,  0.87, 0.036, 0.25, -0.012 }
            }),
            33, // area
            -0.0104719755, // angle
            2.6 // chordLength
    );

    Plane plane = Plane(
        Vec2(0, 100), // pos
        Vec2(100, 0),// velocity
        0, // angle
        51710, // mass
        2027731, // inertia
        
        std::move(wings),
        2, // wingsPoint
        std::move(elevators),
        -15, // elevatorsPoint
        Engine(250000)// engine
    );

    return plane;
}
