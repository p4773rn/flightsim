#include "airfoil.h"
#include "environment.h"
#include <cmath>
#include <iostream>

std::tuple<Vec2, double> Airfoil::getForceAndTorque(
    const Vec2& velocity, 
    double angle, 
    double airDensity) const {

    double angleOfAttack = airfoilAngle
                           + angle
                           + atan2(-velocity.getY(), velocity.getX());

    Table::Entry coefficients = table.get(angleOfAttack);

    double liftMagnitude = airDensity
                           * area 
                           * coefficients.lift 
                           * velocity.lengthSquared();
    double dragMagnitude = -airDensity
                           * area 
                           * coefficients.drag 
                           * velocity.lengthSquared();

    Vec2 direction = velocity.tangent().normalized();
    Vec2 lift = direction * liftMagnitude;
    Vec2 drag = -velocity.normalized() * dragMagnitude;
    Vec2 force = lift + drag;

    double torque = 0.5 * airDensity * velocity.length()
                    * area * chordLength * coefficients.moment;

    return {force, torque};
}

