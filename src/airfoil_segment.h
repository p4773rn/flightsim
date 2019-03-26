#ifndef AIRFOIL_SEGMENT_3D_H
#define AIRFOIL_SEGMENT_3D_H

#include "table.h"
#include <tuple>
#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>

// airplane body coordinates
// relative to center of mass
// x,y,z - right, up, backward

class AirfoilSegment {
public:
    AirfoilSegment( const Table& tableFlaps0,
                    glm::dvec3 pos, 
                    glm::dvec2 angle,
                    double area) :
        tableFlaps0{tableFlaps0},
        pos{pos},
        angle{angle},
        area{area},
        deflection{0},
        flaps{0}
    {}
   
    // args
    //     vel: velocity of center of mass in airplane body coordinates
    //     angVel: angular velocity in airplane body coordinates
    //     height: height of center of mass in world coordinates
    // returns
    //     force: force on center of mass in airplane body coordinates
    //     torque: torque in airplane body coordinates
    std::tuple<glm::dvec3, glm::dvec3> getForceAndTorque(
        const glm::dvec3& vel, 
        const glm::dvec3& angVel, 
        double height,
        std::vector<std::tuple<glm::vec3, glm::vec3, glm::vec3>>& debugArrows) const;

    const glm::dvec2& getDeflection() const { return deflection; }
    void setDeflection(const glm::dvec2& deflection);

    double getFlaps() const { return flaps; }
    void setFlaps(double flaps);

private:
    Table tableFlaps0;
    
    glm::dvec3 pos;
    glm::dvec2 angle; // x - incidence, y - dihedral

    double area;
   
    glm::dvec2 deflection;
    double flaps;
};

#endif
