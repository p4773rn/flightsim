#include "airfoil.h"
#include "environment.h"
#include <cmath>
#include <iostream>

Vec2 Airfoil::getForce( const Vec2& velocity, 
                        double angle, 
                        double airPressure) {
    double angleOfAttack = airfoilAngle
                             + angle
                             + atan2(-velocity.getY(), velocity.getX());

    // std::cout << "Angle of attack: " << (int(angleOfAttack / M_PI * 180) % 360) << std::endl;
    // std::cout << "Airfoil Angle: " << (int(airfoilAngle / M_PI * 180) % 360) << std::endl;
    // std::cout << "Angle: " << (int(angle / M_PI * 180) % 360) << std::endl;
    // std::cout << "Velocity X: " << velocity.getX() << std::endl;
    // std::cout << "Velocity Y: " << velocity.getY() << std::endl << std::endl;
    
    liftMagnitude = airPressure // TODO: think if this is bad code, (probably because if getForce() is not called, then one cannot get liftMagnitude, may be rename into calculateForce() ?)
                * area 
                * table.get(angleOfAttack).lift 
                * velocity.lengthSquared();
    double dragMagnitude = -airPressure
                * area 
                * table.get(angleOfAttack).drag 
                * velocity.lengthSquared();
    // std::cout << "Lift: " << lift << std::endl;
    // std::cout << "Angle: " << angle << std::endl;
    // std::cout << "Angle of attack: " << angleOfAttack << std::endl;

    Vec2 direction = velocity.tangent().normalized();
    Vec2 lift = direction * liftMagnitude;
    Vec2 drag = -velocity.normalized() * dragMagnitude;
    return lift + drag;
}

double Airfoil::getTorque(double airPressure, double angle, const Vec2& velocity) {
    double angleOfAttack = airfoilAngle
                             + angle
                             + atan2(-velocity.getY(), velocity.getX());
    std::cout << "AoA: " << angleOfAttack / M_PI * 180 << std::endl;
    std::cout << "Moment coef: " << table.get(angleOfAttack).moment << std::endl;
    return 0.5 * airPressure * velocity.lengthSquared()
                        * area * chordLength * table.get(angleOfAttack).moment;
}