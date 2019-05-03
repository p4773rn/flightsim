#include "wheels.h"
#include "environment.h"
#include <iostream>
#include "misc.h"
#include <cstdlib>

using std::cout;
using std::endl;

std::tuple<glm::dvec3, glm::dvec3> Wheels::getForceAndTorque(
    const glm::dvec3& velocity, 
    const glm::dvec3& planePosition, 
    const glm::dvec3 angularVelocity, 
    const glm::dquat& orientation, 
    const glm::dvec3& netForce,
    std::vector<std::tuple<glm::vec3, glm::vec3, glm::vec3>>& debugArrows
) const {
    glm::dvec3 direction(0, -2, 0); // offset of the wheel itself relative to where it is attached
    glm::dvec3 absolutePosition = planePosition + orientation * position;

    glm::dvec3 localVelocity = glm::conjugate(orientation) * velocity + glm::cross(angularVelocity, position);
    glm::dvec3 absoluteVelocity = velocity + orientation * glm::cross(angularVelocity, position);


    double h = terrain.get_height(glm::vec2(absolutePosition.x, absolutePosition.z));
    debugArrows.push_back({glm::vec3(absolutePosition.x, h, absolutePosition.z), glm::vec3(0,1,0), glm::vec3(1,0,0)});


    double groundY = h; // ground height at absolutePosition
    //double groundY = 0;
    glm::dvec3 groundNormal = glm::normalize(glm::dvec3(0, 1, 0)); // ground normal at absolutePosition

    if (absolutePosition.y > groundY) {
        return {glm::dvec3(0), glm::dvec3(0)};
    }
    
    if (absolutePosition.y < groundY - 4) {
        //exit(0);
        throw std::runtime_error("Wheels break");
    }

    // how much the spring is compressed
    // not entirely correct
    double springDeltaLength = (groundY - absolutePosition.y);


    //    double normalForceMagnitude =  -absolutePosition.getY() * stiffness - (absoluteSpeed.getY() * springFriction);



    //glm::dvec3 springForce = orientation * (-direction) * (stiffness * springDeltaLength);
    //glm::dvec3 dampingForce = orientation * (-direction) * glm::dot(absoluteVelocity, orientation * direction) * springFriction;


    glm::dvec3 springForce = glm::dvec3(0, (stiffness * springDeltaLength), 0);
    glm::dvec3 dampingForce = glm::dvec3(0, glm::dot(absoluteVelocity, orientation * direction) * springFriction, 0);

    // netForce is divided by 3 because we have 3 Wheel objects
    double normalForceMagnitude = abs(glm::dot(-netForce/3.0, groundNormal));
    glm::dvec3 normalForce = groundNormal * normalForceMagnitude;

   
    // Modeling only kinetic/rolling friction. No static friction
    glm::dvec3 frictionForce(0);
    if (abs(localVelocity.x) > 0.01 || abs(localVelocity.z) > 0.01) {
        //TODO: take ground surface (road/grass) into account
        double frictionCoefficientZ = brakesOn ? 0.7 : 0.095; // friction coefficient when plane is moving forward/backward
        double frictionCoefficientX = 1; // friction coefficient when plane is moving sideways

        
        glm::dvec3 localFrictionForceDirection = glm::normalize(glm::dvec3(-localVelocity.x,0,-localVelocity.z));
        glm::dvec3 frictionForceDirection = orientation * localFrictionForceDirection;

        // interpolating between 2 coefficients depending on direction of movement
        double frictionCoefficient = abs(localFrictionForceDirection.z) * frictionCoefficientZ +
                                     abs(localFrictionForceDirection.x) * frictionCoefficientX;

        double frictionForceMagnitude = normalForceMagnitude * frictionCoefficient;
        frictionForce = frictionForceDirection * frictionForceMagnitude;
        //debugArrows.push_back({glm::vec3(absolutePosition), glm::vec3(frictionForce*0.01), glm::vec3(0,1,1)});
    }
    //std::cout << frictionCoefficient << std::endl;

    // Not using springForce and dampingForce for torque
    // because they are numerically unstable
    /*
    glm::dvec3 force = normalForce + frictionForce;
    force += springForce + dampingForce;
    */
    glm::dvec3 force = springForce + dampingForce + frictionForce;
    glm::dvec3 torque = glm::cross(position, springForce + dampingForce);

    debugArrows.push_back({glm::vec3(absolutePosition), glm::vec3(force*0.01), glm::vec3(0,1,1)});
    
    return {force, torque};
}

//std::tuple<glm::dvec3, glm::dvec3> Wheels::getForceAndTorque(const glm::dvec3& velocity, const glm::vec3& planePosition, const glm::dvec3 angularVelocity, const glm::dquat& orientation , double mass) const {
//    Vec2 force;
//    double torque;
//
//    double pitchAngle = rad2deg(glm::eulerAngles(orientation).x);
//
//    // Position relative to the plane
//    Vec2 relativePosition = Vec2(
//        position.getX() * cos(pitchAngle) - position.getY() * sin(pitchAngle),
//        position.getX() * sin(pitchAngle) + position.getY() * cos(pitchAngle)
//    );
//
//    // Check contact with ground (no contact = no force)
//    if (relativePosition.getY() + planePosition.y > 0) {
//        return { glm::dvec3(0), glm::dvec3(0) };
//    }
//
//    // Speed relative to the plane
//    Vec2 relativeSpeed = Vec2( -angularVelocity.x * relativePosition.getY(), angularVelocity.x * relativePosition.getX());
//
//    // Check for crash
//    
//    // if (relativePosition.getY() + planePosition.y < position.getY()) {
//    //     if (position.getX() < 0) {
//    //         cout << "Main wheels crashed" << endl;
//    //     } else {
//    //         cout << "Front wheels crashed" << endl;
//    //     }
//    //     throw std::runtime_error("Wheels break");
//    // }
//
//    Vec2 absolutePosition = Vec2(relativePosition.getX() - planePosition.z, relativePosition.getY() + planePosition.y);
//    Vec2 absoluteSpeed = Vec2(relativeSpeed.getX() - velocity.z, relativeSpeed.getY() + velocity.y);
//
//    double normalForceMagnitude =  -absolutePosition.getY() * stiffness - (absoluteSpeed.getY() * springFriction);
//
//    // Friction from rolling
//    // TODO: Probably need to refactor this
//    double wheelsRadius = 0.55;
//    double rollingFrictionCoefficient = 0.01;
//    double frictionForceDirection;
//    if (velocity.z < 0) {
//        frictionForceDirection = -1;
//    } else {
//        frictionForceDirection = 1;
//    }
//    if (velocity.z != 0) {
//
//        double gravitationalForceMagnitude = mass * GRAVITATIONAL_ACCELERATION;
//        double rollingFrictionMagnitude =  normalForceMagnitude * rollingFrictionCoefficient / wheelsRadius;
//
//        double axleFrictionCoefficient = 0.001; // Experimental value
//        double numberOfWheels = 8; // TODO: refactor (move this somewhere else)
//        // Divide the number of wheels by two because we have two Wheels objects which in sum will give the correct value (TODO: refactor this)
//        double axleFrictionMagnitude = normalForceMagnitude * (numberOfWheels / 2) * axleFrictionCoefficient;
//        // TODO: Refactor somehow to eliminate the need of (dividing the rolling friction by so for the front wheels and main wheels in sum give the correct magnitude)
//        double frictionForceMagnitude = (rollingFrictionMagnitude + axleFrictionMagnitude) / 2;
//        if (brakesOn > 0) {
//            // TODO: Temp storage of friction coeffs, probably refactor?
//            double slipFrictionCoefficient = 0.6;
//            // ABS braking gives friciton which is not greater than friction from slipping
//            // Assumption: Ideal ABS gives the same friction as slipping (without damaging tires)
//            double brakesFrictionMagnitude = normalForceMagnitude * slipFrictionCoefficient;
//            
//            // 0.8 is the proportion of main wheels to all wheels (ask Alisher)
//            frictionForceMagnitude += brakesFrictionMagnitude - (axleFrictionMagnitude * 0.8);
//        }
//        force.setX(frictionForceDirection * frictionForceMagnitude);
//    }
//    double frictionTorque = position.cross(force);
//
//    // absolutePosition.getY() returns the deformation
//    force.setY(normalForceMagnitude);
//
//    torque = force.getY() * position.getX() + frictionTorque;
//
//    // If main wheels, produce braking torque
//    if (position.getX() < 0) {
//
//    }
//    glm::dvec3 threeDForce = glm::dvec3(0, force.getY(), -force.getX());
//    glm::dvec3 threeDTorque = glm::dvec3(torque, 0, 0);
//
//    return { threeDForce, threeDTorque };
//}
