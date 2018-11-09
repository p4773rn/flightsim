#include "wheels.h"
#include <iostream>

using std::cout;
using std::endl;

std::tuple<Vec2, double> Wheels::getForceAndTorque(const Vec2& velocity, const Vec2& planePosition, double angularVelocity, double angle) const {
    Vec2 force;
    double torque;

    // Position relative to the plane
    Vec2 relativePosition = Vec2(
        position.getX() * cos(angle) - position.getY() * sin(angle),
        position.getX() * sin(angle) + position.getY() * cos(angle)
    );

    // Check contact with ground (no contact = no force)
    if (relativePosition.getY() + planePosition.getY() > 0) {
        return { force, torque };
    }

    // Speed relative to the plane
    Vec2 relativeSpeed = Vec2( -angularVelocity * relativePosition.getY(), angularVelocity * relativePosition.getX());

    // Check for crash
    if (relativePosition.getY() + planePosition.getY() < position.getY()) {
        cout << "Crash Position: " << relativePosition + planePosition << endl;
        cout << "Crash Speed: " << relativeSpeed + velocity << endl;
        if (position.getX() < 0) {
            cout << "Front wheels" << endl;
        } else {
            cout << "Main wheels" << endl;
        }
        throw std::runtime_error("Wheel break");
    }

    Vec2 absolutePosition = relativePosition + planePosition;
    Vec2 absoluteSpeed = relativeSpeed + velocity;

    double normalForceMagnitude =  -absolutePosition.getY() * stiffness - (absoluteSpeed.getY() * springFriction);

    // Hand-made formula
    if (brakesOn) {
        // Temp storage of friction coeffs
        double slipFrictionCoefficient = 0.7;

        double slipFrictionMagnitude = normalForceMagnitude * slipFrictionCoefficient;
        double brakesFrictionMagnitude = 1000000000000;
        force.setX( - std::min(slipFrictionMagnitude, brakesFrictionMagnitude));
    } else {
        force.setX( -absoluteSpeed.getX() * frictionCoefficient);
    }

    // absolutePosition.getY() returns the deformation
    force.setY(normalForceMagnitude);

    torque = force.getY() * position.getX();

    return { force, torque };
}
