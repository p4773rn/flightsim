#ifndef AIRFOIL_SEGMENT_3D_H
#define AIRFOIL_SEGMENT_3D_H

#include "table.h"
#include <tuple>
#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>
#include "openGL/terrain/terrain.h"

// airplane body coordinates
// relative to center of mass
// x,y,z - right, up, backward

class AirfoilSegment {
public:
    AirfoilSegment( const Table& tableFlaps0,
                    glm::dvec3 pos,
                    glm::dvec2 angle,
                    double area,
                    const Terrain& terrain) :
        tableFlaps0{tableFlaps0},
        pos{pos},
        angle{angle},
        area{area},
        deflection{0},
        flaps{0},
        terrain{&terrain}
    {}
   
    // args
    //     vel: velocity of center of mass in world coordinates
    //     angVel: angular velocity in airplane body coordinates
    //     height: height of center of mass in world coordinates
    // returns
    //     force: force on center of mass in world coordinates
    //     torque: torque in airplane body coordinates
    std::tuple<glm::dvec3, glm::dvec3> getForceAndTorque(
        const glm::dvec3& vel,
        const glm::dquat& orientation,
        const glm::dvec3& angVel, 
        const glm::dvec3& planePos,
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

    const Terrain* terrain;
};

#endif
