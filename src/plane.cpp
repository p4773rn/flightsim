#include "plane.h" 
#include "environment.h"
#include "misc.h"
#include <iostream>
#include <tuple>

using std::cout;
using std::endl;

void Plane::update(double delta) {
    double airDensity = getAirDensity(pos.getY());
    
    Vec2 wingsForce, elevatorsForce;
    double wingsTorque, elevatorsTorque;
    
    double height = getHeight();
    std::tie(wingsForce, wingsTorque) = wings.getForceAndTorque(velocity, pitchAngle, airDensity, height);

    Vec2 elevatorsVelocity = {
        velocity.getX() - angularVelocity * (elevatorsPoint.getX() * sin(pitchAngle) + 
                                             elevatorsPoint.getY() * cos(pitchAngle)),
        velocity.getY() + angularVelocity * (elevatorsPoint.getX() * cos(pitchAngle) - 
                                             elevatorsPoint.getY() * sin(pitchAngle))};
    
    std::tie(elevatorsForce, elevatorsTorque) = elevators.getForceAndTorque(elevatorsVelocity, pitchAngle, airDensity, height);
    
    // Forces update
    Vec2 netForce;
    
    // Weight
    Vec2 weight = Vec2(0, -GRAVITATIONAL_ACCELERATION * mass);
    netForce += weight;
    
    // Lift + Drag
    netForce += wingsForce;
    netForce += elevatorsForce;

    // Thrust
    Vec2 thrust = engine.getThrust(pitchAngle);
    netForce += thrust;
   
    // Wheels force
    Vec2 frontWheelsForce, mainWheelsForce;
    double frontWheelsTorque, mainWheelsTorque;

    std::tie(frontWheelsForce, frontWheelsTorque) = frontWheels.getForceAndTorque(velocity, pos, angularVelocity, pitchAngle, mass);
    std::tie(mainWheelsForce, mainWheelsTorque) = mainWheels.getForceAndTorque(velocity, pos, angularVelocity, pitchAngle, mass);

    cout << endl << "FRONT WHEELS FORCE: " << frontWheelsForce << endl << endl;
    cout << endl << "MAIN WHEELS FORCE: " << mainWheelsForce << endl << endl;
    netForce += frontWheelsForce;
    netForce += mainWheelsForce;

    // Straightforward implementation of drag from fuselage
    // TODO, implement fuselage as airfoil (to produce more realistic drag)
    // dynamicPressure * area * coefficients.drag;
    double dynamicPressure = 0.5 * airDensity * velocity.lengthSquared();
    double fuselageArea = 4 * 4; // Simple square for now
    double fuselageDragCoef = 0.11; // Experimentally obtained
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
    pitchAngle += angularVelocity * delta;

    // Making sure pitch angle stays within [-pi, pi] rads
    if (pitchAngle > M_PI)
        pitchAngle -= 2 * M_PI;
    if (pitchAngle < -M_PI)
        pitchAngle += 2 * M_PI;

    // Final calculations
    Vec2 acceleration = netForce / mass;
    velocity += acceleration * delta;
    pos += velocity * delta;

    //cout << pos.getX() << " " << pos.getY() << endl;
    cout << "Pos: " << pos << endl;
    cout << "Vel: " << velocity << endl;
    cout << "Pitch Angle: " << pitchAngle / M_PI * 180 << " deg" << endl;
    cout << "AngVel: " << angularVelocity / M_PI * 180 << " deg/sec" << endl;
    cout << "Wings AoA: " << getWingsAoA() << " deg" << endl;
    cout << "Elevators AoA: " << getElevatorsAoA() << " deg"  << endl;
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
        /*
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
            }), // default flaps position OLD
        */

            // OFFICIAL DATA OF MIDSPAN AIRFOIL OF BOEING-737 FOR R#=50000
            /*
            Table(
            {
                { deg2rad(-8.0),  -0.5,  0.08, 0.25, -0.033},
                { deg2rad(-6.0), -0.42,  0.05, 0.25, -0.028},
                { deg2rad(-4.0),  -0.4,  0.02, 0.25,  -0.02},
                { deg2rad(-2.0), -0.05,  0.01, 0.25,  -0.02},
                {  deg2rad(0.0), -0.05,  0.01, 0.25, -0.025},
                {  deg2rad(2.0),  0.05,  0.01, 0.25,  -0.03},
                {  deg2rad(4.0),  0.55,  0.01, 0.25, -0.028},
                {  deg2rad(6.0),  0.78, 0.018, 0.25, -0.018},
                {  deg2rad(8.0),  0.93, 0.025, 0.25, -0.008},
                { deg2rad(10.0),   1.0,  0.04, 0.25,      0},
                { deg2rad(12.0),   0.8,  0.06, 0.25,   0.01},
                { deg2rad(14.0),   0.2,  0.18, 0.25,  -0.01},
            }),
            */
           // OFFICIAL DATA OF MIDSPAN AIRFOIL OF BOEING-737 FOR R#=200000
            Table(
            {
                { deg2rad(-8.0),  -0.8,  0.027, 0.25, -0.018},
                { deg2rad(-6.0), -0.56,  0.015, 0.25, -0.005},
                { deg2rad(-4.0),  -0.4,  0.012, 0.25, -0.005},
                { deg2rad(-2.0),     0,  0.011, 0.25,  -0.01},
                {  deg2rad(0.0),  0.15,   0.01, 0.25, -0.003},
                {  deg2rad(2.0),   0.3,  0.012, 0.25, -0.005},
                {  deg2rad(4.0),  0.61,  0.014, 0.25, -0.008},
                {  deg2rad(6.0),   0.8,  0.017, 0.25, -0.003},
                {  deg2rad(8.0),  1.05,  0.022, 0.25, 0.0025},
                { deg2rad(10.0),  1.15,  0.027, 0.25,  0.007},
                { deg2rad(12.0),  1.25,  0.032, 0.25,  0.013},
                { deg2rad(14.0),   1.3,   0.05, 0.25,  0.024},
                { deg2rad(16.0),   0.8,   0.12, 0.25,      0},
            }),
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
            105, // area
            deg2rad(1.5), // construction pitch angle
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
                {  14.0 * M_PI / 180,  0.87, 0.036, 0.25, -0.012 },
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
                {  14.0 * M_PI / 180,  0.87, 0.036, 0.25, -0.012 },
            }), // temp solution TODO: refactor airfoil constructor to allow creating airfoils with no flaps
            33, // area
            deg2rad(-2), // construction pitch angle
            2.6 // chordLength
    );

    Plane plane = Plane(
        Vec2(1, 2), // pos
        Vec2(0, 0),// velocity
        0, // pitch angle in rads
        51710, // mass
        2027731, // inertia
        
        std::move(wings),
        Vec2(-0.5, 0), // wingsPoint
        std::move(elevators),
        Vec2(-20, 1), // elevatorsPoint
        Engine(105000), // engine
        Wheels(72467, 10, 10000, Vec2(15, -2)), // Front wheels
        Wheels(434802, 400, 40000, Vec2(-2, -2)) // Main wheels
    );

    return plane;
}
