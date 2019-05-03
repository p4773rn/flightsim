#include "airfoil_segment.h"
#include "environment.h"
#include <iostream>

using std::cout;
using std::endl;

std::tuple<glm::dvec3, glm::dvec3> AirfoilSegment::getForceAndTorque(
    const glm::dvec3& vel, 
    const glm::dquat& orientation,
    const glm::dvec3& angVel, 
    const glm::dvec3& planePos,
    std::vector<std::tuple<glm::vec3, glm::vec3, glm::vec3>>& debugArrows) const {
    float height = planePos.y;
   
    glm::dvec3 segmentVel = glm::conjugate(orientation) * vel + glm::cross(angVel, pos);
   

    glm::dvec2 normalAngle = angle + deflection * glm::radians(11.0);
    glm::dvec3 normal(glm::cos(normalAngle.x) * glm::sin(-normalAngle.y),
                      glm::cos(normalAngle.x) * glm::cos(-normalAngle.y),
                      -glm::sin(normalAngle.x));
    glm::dvec3 dragDir = glm::normalize(-segmentVel);
    glm::dvec3 liftDir = glm::normalize(glm::cross(glm::cross(dragDir, normal), dragDir));

    double aoa = asin(glm::dot(dragDir, normal));
    
    Table::Entry coefficients = tableFlaps0.get(aoa);
    coefficients.lift *= 1 + (flaps / 4);
    coefficients.drag *= 1 + (flaps / 4);
   
    // height of center of mass, not of this segment, but good enough
    double dynamicPressure = 0.5 * getAirDensity(height) * glm::length(segmentVel) * glm::length(segmentVel);


    double groundHeight = terrain->get_height(glm::vec2(planePos.x, planePos.z));

    double liftMagnitude = dynamicPressure * area * coefficients.lift * getGroundEffect(height - groundHeight) * 1.15;
    double dragMagnitude = dynamicPressure * area * coefficients.drag * 1.6;

    glm::dvec3 lift = liftDir * liftMagnitude;
    glm::dvec3 drag = dragDir * dragMagnitude;
    glm::dvec3 force = lift + drag;

    glm::dvec3 torque = glm::cross(pos, force);
    //TODO: calculate pitching moment
    //think what chord length is for an airfoil segment
    //double torque = dynamicPressure * area * chordLength * coefficients.moment;

    
    debugArrows.push_back(std::make_tuple(glm::vec3(pos), glm::vec3(normal*10.0), glm::vec3(1,1,0)));
    debugArrows.push_back(std::make_tuple(glm::vec3(pos), glm::vec3(force*0.001), glm::vec3(1,0,1)));
    
    // glm::normalize returns NaN for 0 vectors
    if (glm::length(segmentVel) < 0.001)
        return {glm::dvec3(0), glm::dvec3(0)};
    
    return {orientation * force, torque};
}


void AirfoilSegment::setDeflection(const glm::dvec2& deflection) {
    this->deflection = glm::clamp(deflection, glm::dvec2(-1), glm::dvec2(1));
}


void AirfoilSegment::setFlaps(double flaps) {
    this->flaps = glm::clamp(flaps, -1.0, 1.0);}








