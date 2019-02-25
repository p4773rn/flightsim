#include "wheels.h"
#include "environment.h"
#include <iostream>

using std::cout;
using std::endl;

std::tuple<Vec2, double> Wheels::getForceAndTorque(const Vec2& velocity, const Vec2& planePosition, double angularVelocity, double pitchAngle, double mass) const {
    Vec2 force;
    double torque;

    // Position relative to the plane
    Vec2 relativePosition = Vec2(
        position.getX() * cos(pitchAngle) - position.getY() * sin(pitchAngle),
        position.getX() * sin(pitchAngle) + position.getY() * cos(pitchAngle)
    );

    // Check contact with ground (no contact = no force)
    if (relativePosition.getY() + planePosition.getY() > 0) {
        return { force, torque };
    }

    // Speed relative to the plane
    Vec2 relativeSpeed = Vec2( -angularVelocity * relativePosition.getY(), angularVelocity * relativePosition.getX());

    // Check for crash
    /*
    if (relativePosition.getY() + planePosition.getY() < position.getY()) {
        cout << "Crash Position: " << relativePosition + planePosition << endl;
        cout << "Crash Speed: " << relativeSpeed + velocity << endl;
        if (position.getX() < 0) {
            cout << "Main wheels crashed" << endl;
        } else {
            cout << "Front wheels crashed" << endl;
        }
        throw std::runtime_error("Wheels break");
    }
*/
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

        double gravitationalForceMagnitude = mass * GRAVITATIONAL_ACCELERATION;
        double rollingFrictionMagnitude =  normalForceMagnitude * rollingFrictionCoefficient / wheelsRadius;

        double axleFrictionCoefficient = 0.009; // Experimental value
        double numberOfWheels = 8; // TODO: refactor (move this somewhere else)
        // Divide the number of wheels by two because we have two Wheels objects which in sum will give the correct value (TODO: refactor this)
        double axleFrictionMagnitude = normalForceMagnitude * (numberOfWheels / 2) * axleFrictionCoefficient;
        // TODO: Refactor somehow to eliminate the need of (dividing the rolling friction by so for the front wheels and main wheels in sum give the correct magnitude)
        double frictionForceMagnitude = (rollingFrictionMagnitude + axleFrictionMagnitude) / 2;
        if (brakesOn > 0) {
            // TODO: Temp storage of friction coeffs, probably refactor?
            double slipFrictionCoefficient = 0.7;
            // ABS braking gives friciton which is not greater than friction from slipping
            // Assumption: Ideal ABS gives the same friction as slipping (without damaging tires)
            double brakesFrictionMagnitude = normalForceMagnitude * slipFrictionCoefficient;
            
            // 0.8 is the proportion of main wheels to all wheels (ask Alisher)
            frictionForceMagnitude += brakesFrictionMagnitude - (axleFrictionMagnitude * 0.8);
        }
        force.setX(frictionForceDirection * frictionForceMagnitude);
    }
    double frictionTorque = position.cross(force);

    // absolutePosition.getY() returns the deformation
    force.setY(normalForceMagnitude);

    torque = force.getY() * position.getX() + frictionTorque;

    // If main wheels, produce braking torque
    if (position.getX() < 0) {

    }

    return { force, torque };
}
