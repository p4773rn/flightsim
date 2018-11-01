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

    Vec2 elevatorsVelocity = {
        velocity.getX() - angularVelocity * (elevatorsPoint.getX() * sin(angle) + 
                                             elevatorsPoint.getY() * cos(angle)),
        velocity.getY() + angularVelocity * (elevatorsPoint.getX() * cos(angle) - 
                                             elevatorsPoint.getY() * sin(angle))};
    
    std::tie(elevatorsForce, elevatorsTorque) = elevators.getForceAndTorque(elevatorsVelocity, angle, airDensity);
    
    // Forces update
    Vec2 netForce;
    
    // Weight
    Vec2 weight = Vec2(0, -GRAVITATIONAL_ACCELERATION * mass);
    netForce += weight;
    
    // Lift + Drag
    netForce += wingsForce;
    netForce += elevatorsForce;

    // Thrust
    Vec2 thrust = engine.getThrust(angle);
    netForce += thrust;
   
    // Check for collision with ground (think about refactoring this)
    Vec2 wheelsForce = frontWheels.getForce(velocity, pos, angularVelocity, angle) + mainWheels.getForce(velocity, pos, angularVelocity, angle);
    cout << endl << "WHEELS FORCE: " << wheelsForce << endl << endl;
    netForce += wheelsForce;

    // Straightforward implementation of drag from fuselage
    Vec2 fuselageDrag;
    fuselageDrag.setX( -velocity.getX() * 100);
    netForce += fuselageDrag;

    // Torque update
    double torque = 0;
    
    torque += wingsTorque + wingsPoint.cross(wingsForce);
    torque += elevatorsTorque + elevatorsPoint.cross(elevatorsForce);


    double angularAcceleration = torque / inertia;
    angularVelocity += angularAcceleration * delta;   
    angle += angularVelocity * delta;
   

    // Final calculations
    Vec2 acceleration = netForce / mass;
    velocity += acceleration * delta;
    pos += velocity * delta;

    //cout << pos.getX() << " " << pos.getY() << endl;
    cout << "Pos: " << pos << endl;
    cout << "Vel: " << velocity << endl;
    cout << "Angle: " << angle / M_PI * 180 << " deg" << endl;
    cout << "AngVel: " << angularVelocity / M_PI * 180 << " deg/sec" << endl;
    cout << "Wings AoA: " << wings.getAngleOfAttack(velocity, angle) / M_PI * 180 << " deg" << endl;
    cout << "Elevators AoA: " << elevators.getAngleOfAttack(velocity, angle) / M_PI * 180 << " deg"  << endl;
    cout << "--------------------------------------" << endl;
    cout << "Gravity: " << weight << endl;
    cout << "Wings: " << wingsForce << " " << wingsTorque + wingsPoint.cross(wingsForce) << endl;
    cout << "Engines: " << thrust << endl;
    cout << "Elevators: " << elevatorsForce << " " << elevatorsTorque + elevatorsPoint.cross(elevatorsForce) << endl;
    cout << "Total: " << netForce << " " << torque << endl;
    cout << "--------------------------------------" << endl;
    cout << "Throttle: " << engine.getThrottle() << endl;
    cout << "Elevator Deflection: " << elevators.getDeflection() << endl;
    cout << "Breaks on/off: " << mainWheels.getBreaksStatus() << endl;
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
            1050, // area
            0.0523598775, // angle
            9 // chordLength
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
            330, // area
            -0.0104719755, // angle
            5 // chordLength
    );

    Plane plane = Plane(
        Vec2(0, 5), // pos
        Vec2(1, 0),// velocity
        0, // angle
        150000, // mass
        2027731, // inertia
        
        std::move(wings),
        Vec2(2, -1), // wingsPoint
        std::move(elevators),
        Vec2(-15, 1), // elevatorsPoint
        Engine(500000), // engine
        Wheels(182467, 10, 20000, Vec2(-35, -5)), // Front wheels
        Wheels(1034802, 400, 80000, Vec2(5, -5)) // Main wheels
    );

    return plane;
}
