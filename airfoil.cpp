#include "airfoil.h"
#include "environment.h"

std::tuple<Vec2, double> Airfoil::getForceAndTorque(
    const Vec2& velocity, 
    double angle, 
    double airDensity) const {

    Table::Entry coefficients = table.get(getAngleOfAttack(velocity, angle));

    double dynamicPressure = 0.5 * airDensity * velocity.lengthSquared();


    double liftMagnitude = dynamicPressure * area * coefficients.lift;
    double dragMagnitude = dynamicPressure * area * coefficients.drag;

    Vec2 lift = velocity.tangent().normalized() * liftMagnitude;
    Vec2 drag = -velocity.normalized() * dragMagnitude;
    Vec2 force = lift + drag;

    double torque = dynamicPressure * area * chordLength * coefficients.moment;

    return {force, torque};
}


