#include "wheels.h"
#include "environment.h"
#include <iostream>

using std::cout;
using std::endl;

std::tuple<Vec2, double> Wheels::getForceAndTorque(const Vec2& velocity, const Vec2& planePosition, double angularVelocity, double angle, double mass) const {
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
        throw std::runtime_error("Wheels break");
    }

    Vec2 absolutePosition = relativePosition + planePosition;
    Vec2 absoluteSpeed = relativeSpeed + velocity;

    double normalForceMagnitude =  -absolutePosition.getY() * stiffness - (absoluteSpeed.getY() * springFriction);

    // Friction from rolling
    // TODO: Probably need to refactor this
    double wheelsRadius = 0.55;
    double rollingFrictionCoefficient = 0.01;
    double frictionForceDirection;
    if (velocity.getX() > 0) {
        frictionForceDirection = -1;
    } else {
        frictionForceDirection = 1;
    }
    if (velocity.getX() != 0) {
        // TODO: Refactor somehow to eliminate the need of (dividing the rolling friction by so for the front wheels and main wheels in sum give the correct magnitude)
        // 
        force.setX( frictionForceDirection * mass * GRAVITATIONAL_ACCELERATION * rollingFrictionCoefficient / (wheelsRadius * 2));
        if (brakesOn > 0) {
            // TODO: Temp storage of friction coeffs, probably refactor?
            double slipFrictionCoefficient = 0.7;
            // ABS braking gives friciton which is not greater than friction from slipping
            // Assumption: Ideal ABS gives the same friction as slipping (without damaging tires)
            double brakesFrictionMagnitude = normalForceMagnitude * slipFrictionCoefficient;

            force.setX(force.getX() + (frictionForceDirection * brakesFrictionMagnitude));
        }
    }

    // absolutePosition.getY() returns the deformation
    force.setY(normalForceMagnitude);

    torque = force.getY() * position.getX();

    return { force, torque };
}
