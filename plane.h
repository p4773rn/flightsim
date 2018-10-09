#ifndef PLANE_H
#define PLANE_H

#include "airfoil.h"
#include "vec2.h"
#include "engine.h"

class Plane {
public:
    Plane(Vec2 pos,
          Vec2 velocity,
          double angle,
          double mass,
          double inertia,

          Airfoil wings,
          double wingsPoint,
          Airfoil elevators,
          double elevatorsPoint,
          Engine engine) :
          
          pos(pos),
          velocity(velocity),
          angle(angle),
          angularVelocity(0),
          mass(mass),
          inertia(inertia),
          wings(wings),
          wingsPoint(wingsPoint),
          elevators(elevators),
          elevatorsPoint(elevatorsPoint),
          engine(engine)
    {}
    

    double getHeight() const {return pos.getY();}
    const Vec2& getPos() const {return pos;}
    void update(double delta);

private:
    Vec2 pos; // For now, xy coordinates of the plane
    Vec2 velocity;
    double angle; // angle of plane relative to horizon
    double angularVelocity;
    double mass;
    double inertia;

    double wingsPoint; // distance from wingss' force to the mass center
    Airfoil wings;

    double elevatorsPoint;
    Airfoil elevators;

    Engine engine;
};


#endif
