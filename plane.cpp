#include "plane.h" 
#include "environment.h"
#include <iostream>
#include <tuple>
#include "misc.h"

using std::cout;
using std::endl;

void Plane::update(double delta) {
    double airDensity = getAirDensity(pos.getY());
    
    Vec2 wingsForce, elevatorsForce, elevatorsForce0, elevatorsForce1;
    double wingsTorque, elevatorsTorque, elevatorsTorque0, elevatorsTorque1;
    
    std::tie(wingsForce, wingsTorque) = wings.getForceAndTorque(velocity, angle, airDensity);

    Vec2 elevatorsVelocity = {
        velocity.getX() - angularVelocity * (elevatorsPoint.getX() * sin(angle) + 
                                             elevatorsPoint.getY() * cos(angle)),
        velocity.getY() + angularVelocity * (elevatorsPoint.getX() * cos(angle) - 
                                             elevatorsPoint.getY() * sin(angle))};
    
    std::tie(elevatorsForce0, elevatorsTorque0) = elevators0.getForceAndTorque(elevatorsVelocity, angle, airDensity);
    std::tie(elevatorsForce1, elevatorsTorque1) = elevators1.getForceAndTorque(elevatorsVelocity, angle, airDensity);
    elevatorsForce = lerp(elevatorsForce0, elevatorsForce1, elevatorDeflection);
    elevatorsTorque = lerp(elevatorsTorque0, elevatorsTorque1, elevatorDeflection);

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
    cout << "Angle: " << rad2deg(angle) << " deg" << endl;
    cout << "AngVel: " << rad2deg(angularVelocity) << " deg/sec" << endl;
    cout << "Wings AoA: " << rad2deg(wings.getAngleOfAttack(velocity, angle)) << " deg" << endl;
    cout << "Elevators AoA: " << rad2deg(elevators0.getAngleOfAttack(elevatorsVelocity, angle)) << " deg"  << endl;
    cout << "--------------------------------------" << endl;
    cout << "Gravity: " << weight << endl;
    cout << "Wings: " << wingsForce << " " << wingsTorque + wingsPoint.cross(wingsForce) << endl;
    cout << "Engines: " << thrust << endl;
    cout << "Elevators: " << elevatorsForce << " " << elevatorsTorque + elevatorsPoint.cross(elevatorsForce) << endl;
    cout << "Total: " << netForce << " " << torque << endl;
    cout << "--------------------------------------" << endl;
    cout << "Throttle: " << engine.getThrottle() << endl;
    cout << "Elevator Deflection: " << elevatorDeflection << endl;
    cout << "Elevator Angle Offset: " << rad2deg(elevators0.getAirfoilAngleOffset()) << endl;
    cout << "Breaks on/off: " << mainWheels.getBreaksStatus() << endl;
    cout << endl;
}


void Plane::setElevatorDeflection(double deflection) {
    elevatorDeflection = clamp(deflection, 0, 1);
}

void Plane::setElevatorAngleOffset(double angleOffset) {
    angleOffset = clamp(angleOffset, -deg2rad(5.0), deg2rad(5.0));

    elevators0.setAirfoilAngleOffset(angleOffset);
    elevators1.setAirfoilAngleOffset(angleOffset);
}

Plane Plane::getDefaultPlane() {
    Airfoil wings(
            Table(
            {
                { deg2rad(-8.0), -0.45, 0.022, 0.246, -0.097 },
                { deg2rad(-6.0), -0.23, 0.014, 0.246, -0.092 },
                { deg2rad(-4.0), -0.03, 0.012, 0.246, -0.092 },
                { deg2rad(-2.0),  0.20, 0.010, 0.246, -0.092 },
                { deg2rad( 0.0),  0.38, 0.010, 0.246, -0.093 },
                { deg2rad( 2.0),  0.60, 0.010, 0.246, -0.095 },
                { deg2rad( 4.0),  0.80, 0.012, 0.246, -0.098 },
                { deg2rad( 6.0),  1.00, 0.014, 0.246, -0.100 },
                { deg2rad( 8.0),  1.15, 0.017, 0.246, -0.100 },
                { deg2rad(10.0),  1.27, 0.022, 0.246, -0.095 },
                { deg2rad(12.0),  1.36, 0.030, 0.246, -0.092 },
                { deg2rad(14.0),  1.35, 0.042, 0.246, -0.092 },
                { deg2rad(16.0),  1.25, 0.059, 0.246, -0.095 }
            }),
            1050, // area
            0.0523598775, // angle
            9 // chordLength
    );

    Airfoil elevators0(
            Table(
            {
                { deg2rad(-14.0), -0.87, 0.036, 0.25,  0.012 },
                { deg2rad(-12.0), -0.89, 0.028, 0.25,  0.004 },
                { deg2rad(-10.0), -0.90, 0.021, 0.25,  0.002 },
                { deg2rad( -8.0), -0.85, 0.018, 0.25,  0.0   },
                { deg2rad( -6.0), -0.64, 0.014, 0.25,  0.0   },
                { deg2rad( -4.0), -0.43, 0.011, 0.25,  0.0   },
                { deg2rad( -2.0), -0.21, 0.010, 0.25,  0.0   },
                { deg2rad(  0.0),   0.0, 0.009, 0.25,  0.0   },
                { deg2rad(  2.0),  0.21, 0.010, 0.25,  0.0   },
                { deg2rad(  4.0),  0.43, 0.011, 0.25,  0.0   },
                { deg2rad(  6.0),  0.64, 0.014, 0.25,  0.0   },
                { deg2rad(  8.0),  0.85, 0.018, 0.25,  0.0   },
                { deg2rad( 10.0),  0.90, 0.021, 0.25, -0.002 },
                { deg2rad( 12.0),  0.89, 0.028, 0.25, -0.004 },
                { deg2rad( 14.0),  0.87, 0.036, 0.25, -0.012 }
            }),
            330, // area
            -0.0104719755, // angle
            5 // chordLength
    );

    Airfoil elevators1(
            Table(
            {
                { deg2rad(-8.0),    0.90,    0.022,  0.25,   -0.287 },
                { deg2rad(-6.0),    1.12,    0.014,  0.25,   -0.297 },
                { deg2rad(-4.0),    1.34,    0.012,  0.25,   -0.302 },
                { deg2rad(-2.0),    1.56,    0.010,  0.25,   -0.305 },
                { deg2rad( 0.0),    1.75,    0.010,  0.25,   -0.305 },
                { deg2rad( 2.0),    1.95,    0.010,  0.25,   -0.305 },
                { deg2rad( 4.0),    2.14,    0.012,  0.25,   -0.305 },
                { deg2rad( 6.0),    2.43,    0.014,  0.25,   -0.302 },
                { deg2rad( 8.0),    2.50,    0.017,  0.25,   -0.300 },
                { deg2rad(10.0),    2.65,    0.022,  0.25,   -0.290 },
                { deg2rad(12.0),    2.63,    0.030,  0.25,   -0.275 }
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
        std::move(elevators0),
        std::move(elevators1),
        Vec2(-15, 1), // elevatorsPoint
        Engine(500000), // engine
        Wheels(182467, 10, 20000, Vec2(-35, -5)), // Front wheels
        Wheels(1034802, 400, 80000, Vec2(5, -5)) // Main wheels
    );

    return plane;
}
