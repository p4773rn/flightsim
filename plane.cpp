#include "plane.h" 
#include "environment.h"
#include <iostream>

void Plane::update(double delta) {
    double airPressure = getAirPressure(pos.getY());
    // Forces update

    Vec2 netForce;
    
    // Weight
    netForce += Vec2(0, -GRAVITATIONAL_ACCELERATION * mass);
    
    // Lift + Drag
    Vec2 wingsForce = wing.getForce(velocity, angle, airPressure);
    netForce += wingsForce;

    // Thrust
    netForce += engine.getThrust(angle);


    velocity += netForce / mass * delta;
    pos += velocity * delta;

   
    // Torque update
    // this needs to be redone accoring to right formula
    //      angularVelocity = wing.getLiftMagnitude() / (mass * wingsPoint) * delta;
    // wingsForce = mass * wingsPoint * angularAcceleration
    // L = q.S.CL(α)
    double torque = wing.getTorque(airPressure, angle, velocity);
    torque += wingsForce.getY() * wingsPoint;
    double angularAcceleration = torque / inertia;
    angularVelocity += angularAcceleration * delta;   

    // std::cout << "Lift: " << wing.getLiftMagnitude() << std::endl;
    // std::cout << "angVel: " << angularVelocity << std::endl << std::endl;
    
    angle += angularVelocity * delta;

    std::cout << "Velocity X: " << velocity.getX() << std::endl;
    std::cout << "Velocity Y: " << velocity.getY() << std::endl << std::endl;
    std::cout << "Net X: " << netForce.getX() << std::endl;
    std::cout << "Net Y: " << netForce.getY() << std::endl;
    std::cout << "Torque: " << torque << "; Angular velocity: " << angularVelocity << std::endl;
    std::cout << "Angle: " << angle << std::endl;
    std::cout << std::endl;

    //kod Hansa
    //double q = 0.5 * rho * v0. norm( );
    //double t = q * wing_surface * wing_chord * c. moment; 
    //t = t + wing_position.x * f.y - wing_position.y * f.x;

   
//   twodimensional b737( "B737-300", 51710.0, 2027731.0, 1.225,
//      { 0, 1000, 0 }, 1.5_deg,    // position and attitude in LRU.
//      { 100, 0, 0 }, 0.0,          // speed and omega in LRU. 
//      airfoils::fuselage( ),
//      { 16, 0, 0 },
//      { 12, 128, 0 }, 32,         // Fuselage surfaces, fuselage length.
//      airfoils::naca4412( ), 
//      { 2, -1, 0 }, 3.0_deg,      // Wing position, wing theta.
//      103.0, 3.6,                 // Wing surface, wing chord.
//      { 2, -2, 0 },
//      0.1, 0.1,                     // Engine throttle, engine power.
//      airfoils::naca0009( ),           // Aileron undeflected.
//      airfoils::naca4412withflap( ),   // Aileron deflected.
//      -0.0,                             // Current deflection.
//      { -15, 1, 0 }, -0.6_deg,    // Elevator position, elevator theta. 
//      33, 2.6 );                   // Elevator surface, elevator chord.
  
}

/*
g++ *.cpp -o fs.out -lsfml-graphics -lsfml-window -lsfml-system
/tmp/ccujh0Ov.o: In function `getAirPressure(double)':
plane.cpp:(.text+0x0): multiple definition of `getAirPressure(double)'
/tmp/ccqd9xta.o:airfoil.cpp:(.text+0x0): first defined here
collect2: error: ld returned 1 exit status
Makefile:5: recipe for target 'all' failed
make: *** [all] Error 1

*/