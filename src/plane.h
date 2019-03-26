#ifndef PLANE_H
#define PLANE_H

#include "airfoil_segment.h"
#include <glm/glm.hpp>
#include "engine.h"
#include "wheels.h"

class Plane {
public:
    Plane(glm::dvec3 pos,
          glm::dvec3 vel,
          double mass,
          glm::dmat3 inertia,

          std::vector<AirfoilSegment> wings,
          std::vector<AirfoilSegment> elevators,
          std::vector<AirfoilSegment> rudder,
          std::vector<AirfoilSegment> fuselage,
          Engine engine,
          Wheels frontWheels,
          Wheels mainWheels) :
          
          pos{pos},
          vel{vel},
          orientation{0, 0, 0, 0},
          angVel{0},
          mass{mass},
          inertia{inertia},
          wings{wings},
          elevators{elevators},
          rudder{rudder},
          fuselage{fuselage},
          engine{engine},
          frontWheels{frontWheels},
          mainWheels{mainWheels}
    {}
    
    
    Plane(const Plane& other) = default;
    Plane(Plane&& other) = default;

    double getHeight() const {return pos.y;}
    const glm::dvec3& getPos() const {return pos;}
    //const glm::dvec3& getAngles() const {return angles;}
    const glm::dvec3& getVel() const {return vel;}
    void update(double delta,
        std::vector<std::tuple<glm::vec3, glm::vec3, glm::vec3>>& debugArrows);

    int getThrottle() const {return engine.getThrottle();}
    void setThrottle(int throttle) {engine.setThrottle(throttle);}

    double getRudderDeflection() const {return rudder.back().getDeflection().x;}
    void setRudderDeflection(double deflection) {
        for (auto& s : rudder)
            s.setDeflection(glm::dvec2(deflection, 0));
        
    }

    double getElevatorDeflection() const {return elevators.back().getDeflection().x;}
    void setElevatorDeflection(double deflection) {
        for (auto& s : elevators)
            s.setDeflection(glm::dvec2(deflection, 0));
        
    }

    double getFlaps() const {return wings.front().getFlaps();}
    void setFlaps(double flapsPosition) {
        for (size_t i = 0; i < wings.size()/2; ++i) {
            wings[i].setFlaps(flapsPosition);
            wings[wings.size()-1-i].setFlaps(-flapsPosition);
        }
    }
    void toggleBrakes() {
        mainWheels.toggleBrakes();
        frontWheels.toggleBrakes();
    }
    bool getBrakesStatus() const {return mainWheels.getBrakesStatus();}

    static Plane getDefaultPlane(std::vector<std::tuple<glm::vec3, glm::vec3, glm::vec3>>& debugArrows);

    // doesn't take into account incidence, but hopefully it is small enough for estimating inertia
    static void addPointMasses(double length, double root, double tip, double sweep, 
                               const glm::dvec3& pos, double dihedral, bool invertX,
                               std::vector<glm::dvec3>& pointMasses);
    static std::vector<AirfoilSegment> createAirfoilSegments(
                               int num_segments,
                               double length, double root, double tip, double sweep, 
                               const glm::dvec3& pos, double incidence, double dihedral, bool invertX,
                               const Table& tableFlaps0);
private:
    glm::dvec3 pos; // For now, xy coordinates of the plane
    glm::dvec3 vel;
    glm::dquat orientation;
    glm::dvec3 angVel;
    double mass;
    glm::dmat3 inertia;

    std::vector<AirfoilSegment> wings;
    std::vector<AirfoilSegment> elevators;
    std::vector<AirfoilSegment> rudder;
    std::vector<AirfoilSegment> fuselage;

    Engine engine;

    Wheels frontWheels;
    Wheels mainWheels;
};


#endif
