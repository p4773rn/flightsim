#include "airfoil.h"
#include <cmath>
#include <iostream>

Vec2 Airfoil::getLift( const Vec2& velocity, 
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
    
    double lift = airPressure
                * area 
                * table.get(angleOfAttack).lift 
                * velocity.lengthSquared();

    // std::cout << "Lift: " << lift << std::endl;
    // std::cout << "Angle: " << angle << std::endl;
    // std::cout << "Angle of attack: " << angleOfAttack << std::endl;

    Vec2 direction = velocity.tangent().normalized();
    return direction * lift;
}
