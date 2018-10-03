#include "airfoil.h"
#include <cmath>
#include <iostream>

Vec2 Airfoil::getForce( const Vec2& velocity, 
                        double angle, 
                        double airPressure) {
    double angleOfAttack = airfoilAngle
                             + angle
                             + atan2(-velocity.getY(), velocity.getX());

    std::cout << "Angle of attack: " << angleOfAttack << std::endl;
    std::cout << "Airfoil Angle: " << airfoilAngle << std::endl;
    std::cout << "Angle: " << angle << std::endl;
    std::cout << "Velocity X: " << velocity.getX() << std::endl;
    std::cout << "Velocity Y: " << velocity.getY() << std::endl << std::endl;
    
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

    // Vec2 direction = velocity.tangent().normalized();
    Vec2 lift = Vec2(liftMagnitude * cos(angle + (M_PI / 2)), liftMagnitude * sin(angle + (M_PI / 2)));

    Vec2 drag = Vec2( - dragMagnitude * cos(angle), - dragMagnitude * sin(angle));
    return lift + drag;
}
