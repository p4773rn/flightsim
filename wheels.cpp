#include "wheels.h"
#include <iostream>

using std::cout;
using std::endl;

Vec2 Wheels::getForce(const Vec2& velocity, const Vec2& planePosition, double angularVelocity, double angle) const {
    Vec2 force;

    // Position relative to the plane
    Vec2 relativePosition = Vec2(
        position.getX() * cos(angle) - position.getY() * sin(angle),
        position.getX() * sin(angle) + position.getY() * cos(angle)
    );

    // Check contact with ground (no contact = no force)
    if (relativePosition.getY() + planePosition.getY() > 0) {
        return force;
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

    // Hand-made formula
    force.setX( -absoluteSpeed.getX() * frictionCoefficient);

    // absolutePosition.getY() returns the deformation
    force.setY( -absolutePosition.getY() * stiffness - (absoluteSpeed.getY() * springFriction));

    return force;
}
