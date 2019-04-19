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
          Wheels mainWheelsL,
          Wheels mainWheelsR) :
          
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
          mainWheelsL{mainWheelsL},
          mainWheelsR{mainWheelsR}
    {}
    
    
    Plane(const Plane& other) = default;
    Plane(Plane&& other) = default;
    void update(double delta, std::vector<std::tuple<glm::vec3, glm::vec3, glm::vec3>>& debugArrows);


    const glm::dvec3& getPos() const {return pos;}
    const glm::dvec3& getVelocity() const {return vel;}
    int getThrottle() const {return engine.getThrottle();}
    double getHeight() const {return pos.y;}
    glm::dvec3 getAngles() const {return glm::eulerAngles(orientation);}
    glm::dquat& getOrientation() {return orientation;}
    double getRudderDeflection() const {return rudder.back().getDeflection().x;}
    double getElevatorDeflection() const {return elevators.back().getDeflection().x;}
    double getFlaps() const {return wings.front().getFlaps();}
    bool getBrakesStatus() const {return mainWheelsL.getBrakesStatus();}
    double getPitchAngle() const;
    double getYawAngle() const;
    
    void setRudderDeflection(double deflection) {
        for (auto& s : rudder)
        	s.setDeflection(glm::dvec2(deflection, 0));
    }
    
    void setThrottle(int throttle) {engine.setThrottle(throttle);}

    void setElevatorDeflection(double deflection) {
        for (auto& s : elevators)
            s.setDeflection(glm::dvec2(deflection, 0));
        
    }

    void setFlaps(double flapsPosition) {
        for (size_t i = 0; i < wings.size()/2; ++i) {
            wings[i].setFlaps(flapsPosition);
            wings[wings.size()-1-i].setFlaps(-flapsPosition);
        }
    }
    
    void toggleBrakes() {
        mainWheelsL.toggleBrakes();
        mainWheelsR.toggleBrakes();
        frontWheels.toggleBrakes();
    }
    

    static Plane getDefaultPlane(std::vector<std::tuple<glm::vec3, glm::vec3, glm::vec3>>& debugArrows, const Terrain& terrain);

    // doesn't take into account incidence, but hopefully it is small enough for estimating inertia
    static void addPointMasses(double length, double root, double tip, double sweep, 
                               const glm::dvec3& pos, double dihedral, bool invertX,
                               std::vector<glm::dvec3>& pointMasses);
    static std::vector<AirfoilSegment> createAirfoilSegments(
                               int num_segments,
                               double length, double root, double tip, double sweep, 
                               const glm::dvec3& pos, double incidence, double dihedral, bool invertX,
                               const Table& tableFlaps0, const Terrain& terrain);
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
    Wheels mainWheelsL;
    Wheels mainWheelsR;
};


#endif
