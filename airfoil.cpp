#include "airfoil.h"
#include <cmath>

Vec2 Airfoil::getForce( const Vec2& velocity, 
                        double angle, 
                        double airPressure) {
    double angleOfAttack = airfoilAngle
                             + angle
                             + atan2(-velocity.getY(), velocity.getX());
    double lift = airPressure
                * area 
                * table.get(angleOfAttack).lift 
                * velocity.lengthSquared();
    Vec2 direction = velocity.tangent().normalized();
    return direction * lift;
}
