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
    
    double height = getHeight();
    std::tie(wingsForce, wingsTorque) = wings.getForceAndTorque(velocity, angle, airDensity, height);

    Vec2 elevatorsVelocity = {
        velocity.getX() - angularVelocity * (elevatorsPoint.getX() * sin(angle) + 
                                             elevatorsPoint.getY() * cos(angle)),
        velocity.getY() + angularVelocity * (elevatorsPoint.getX() * cos(angle) - 
                                             elevatorsPoint.getY() * sin(angle))};
    
    std::tie(elevatorsForce, elevatorsTorque) = elevators.getForceAndTorque(elevatorsVelocity, angle, airDensity, height);
    
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
   
    // Wheels force
    Vec2 frontWheelsForce, mainWheelsForce;
    double frontWheelsTorque, mainWheelsTorque;

    std::tie(frontWheelsForce, frontWheelsTorque) = frontWheels.getForceAndTorque(velocity, pos, angularVelocity, angle, mass);
    std::tie(mainWheelsForce, mainWheelsTorque) = mainWheels.getForceAndTorque(velocity, pos, angularVelocity, angle, mass);

    cout << endl << "FRONT WHEELS FORCE: " << frontWheelsForce << endl << endl;
    cout << endl << "MAIN WHEELS FORCE: " << mainWheelsForce << endl << endl;
    netForce += frontWheelsForce;
    netForce += mainWheelsForce;

    // Straightforward implementation of drag from fuselage
    // TODO, implement fuselage as airfoil (to produce more realistic drag)
    // dynamicPressure * area * coefficients.drag;
    double dynamicPressure = 0.5 * airDensity * velocity.lengthSquared();
    double fuselageArea = 4 * 4; // Simple square for now
    double fuselageDragCoef = 1.4; // Experimentally obtained
    double fuselageDragMagnitude = dynamicPressure * fuselageArea * fuselageDragCoef;
    Vec2 fuselageDrag = - velocity.normalized() * fuselageDragMagnitude;
    netForce += fuselageDrag;

    // Torque update
    double torque = 0;
    
    torque += wingsTorque + wingsPoint.cross(wingsForce);
    torque += elevatorsTorque + elevatorsPoint.cross(elevatorsForce);

    // Wheels torque
    torque += frontWheelsTorque + mainWheelsTorque;

    double angularAcceleration = torque / inertia;
    angularVelocity += angularAcceleration * delta;   
    angle += angularVelocity * delta;

    // Making sure angle stays within [-pi, pi] rads
    if (angle > M_PI)
        angle -= 2 * M_PI;
    if (angle < -M_PI)
        angle += 2 * M_PI;

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
    cout << "Fuselage: " << fuselageDrag << endl;
    cout << "Total: " << netForce << " " << torque << endl;
    cout << "--------------------------------------" << endl;
    cout << "Air density: " << airDensity << endl;
    cout << "Ground effect multiplier: " << getGroundEffect(height) << endl;
    cout << "--------------------------------------" << endl;
    cout << "Throttle: " << engine.getThrottle() << endl;
    cout << "Elevator Deflection: " << elevators.getDeflection() << endl;
    cout << "Flaps position: " << wings.getFlaps() << endl;
    cout << "Brakes on/off: " << mainWheels.getBrakesStatus() << endl;
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
            }), // default flaps position 
            /*
                Original lift old drag
                { -8.0 * M_PI / 180,    0.90,    0.055,  0.25,   -0.287 },
                { -6.0 * M_PI / 180,    1.12,    0.035,  0.25,   -0.297 },
                { -4.0 * M_PI / 180,    1.34,    0.030,  0.25,   -0.302 },
                { -2.0 * M_PI / 180,    1.56,    0.025,  0.25,   -0.305 },
                {  0.0 * M_PI / 180,    1.75,    0.025,  0.25,   -0.305 },
                {  2.0 * M_PI / 180,    1.95,    0.025,  0.25,   -0.305 },
                {  4.0 * M_PI / 180,    2.14,    0.030,  0.25,   -0.305 },
                {  6.0 * M_PI / 180,    2.43,    0.035,  0.25,   -0.302 },
                {  8.0 * M_PI / 180,    2.50,    0.043,  0.25,   -0.300 },
                { 10.0 * M_PI / 180,    2.65,    0.055,  0.25,   -0.290 },
                { 12.0 * M_PI / 180,    2.63,    0.075,  0.25,   -0.275 }
                
                Old lift new drag
                { -8.0 * M_PI / 180,    0.81,    0.082,  0.25,   -0.287 },
                { -6.0 * M_PI / 180,    1.00,    0.052,  0.25,   -0.297 },
                { -4.0 * M_PI / 180,    1.20,    0.045,  0.25,   -0.302 },
                { -2.0 * M_PI / 180,    1.40,    0.037,  0.25,   -0.305 },
                {  0.0 * M_PI / 180,    1.57,    0.037,  0.25,   -0.305 },
                {  2.0 * M_PI / 180,    1.75,    0.037,  0.25,   -0.305 },
                {  4.0 * M_PI / 180,    1.92,    0.045,  0.25,   -0.305 },
                {  6.0 * M_PI / 180,    2.19,    0.052,  0.25,   -0.302 },
                {  8.0 * M_PI / 180,    2.50,    0.064,  0.25,   -0.300 },
                { 10.0 * M_PI / 180,    2.25,    0.082,  0.25,   -0.290 },
                { 12.0 * M_PI / 180,    2.24,    0.112,  0.25,   -0.275 }

                Second lift, drag unchanged from previous
                { -8.0 * M_PI / 180,    0.65,    0.082,  0.25,   -0.287 },
                { -6.0 * M_PI / 180,    0.80,    0.052,  0.25,   -0.297 },
                { -4.0 * M_PI / 180,    0.96,    0.045,  0.25,   -0.302 },
                { -2.0 * M_PI / 180,    1.12,    0.037,  0.25,   -0.305 },
                {  0.0 * M_PI / 180,    1.26,    0.037,  0.25,   -0.305 },
                {  2.0 * M_PI / 180,    1.4,    0.037,  0.25,   -0.305 },
                {  4.0 * M_PI / 180,    1.54,    0.045,  0.25,   -0.305 },
                {  6.0 * M_PI / 180,    1.75,    0.052,  0.25,   -0.302 },
                {  8.0 * M_PI / 180,    2.00,    0.064,  0.25,   -0.300 },
                { 10.0 * M_PI / 180,    1.80,    0.082,  0.25,   -0.290 },
                { 12.0 * M_PI / 180,    1.79,    0.112,  0.25,   -0.275 }
            */
            Table(
            {
                { -8.0 * M_PI / 180,    0.55,    0.082,  0.25,   -0.287 },
                { -6.0 * M_PI / 180,    0.68,    0.052,  0.25,   -0.297 },
                { -4.0 * M_PI / 180,    0.81,    0.045,  0.25,   -0.302 },
                { -2.0 * M_PI / 180,    0.95,    0.037,  0.25,   -0.305 },
                {  0.0 * M_PI / 180,    1.07,    0.037,  0.25,   -0.305 },
                {  2.0 * M_PI / 180,    1.19,    0.037,  0.25,   -0.305 },
                {  4.0 * M_PI / 180,    1.31,    0.045,  0.25,   -0.305 },
                {  6.0 * M_PI / 180,    1.49,    0.052,  0.25,   -0.302 },
                {  8.0 * M_PI / 180,    1.70,    0.064,  0.25,   -0.300 },
                { 10.0 * M_PI / 180,    1.53,    0.082,  0.25,   -0.290 },
                { 12.0 * M_PI / 180,    1.52,    0.112,  0.25,   -0.275 }
            }), // take-off flaps position coeffs
            103, // area
            0.0623598775, // angle
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
            }), // temp solution TODO: refactor airfoil constructor to allow creating airfoils with no flaps
            33, // area
            -0.0104719755, // angle
            2.6 // chordLength
    );

    Plane plane = Plane(
        Vec2(1, 4), // pos
        Vec2(1, 0),// velocity
        0, // angle in rads
        51710, // mass
        2027731, // inertia
        
        std::move(wings),
        Vec2(-1, 0), // wingsPoint
        std::move(elevators),
        Vec2(-15, 1), // elevatorsPoint
        Engine(105000), // engine
        Wheels(72467, 10, 10000, Vec2(15, -2)), // Front wheels
        Wheels(434802, 400, 40000, Vec2(-2, -2)) // Main wheels
    );

    return plane;
}
