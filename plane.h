#ifndef PLANE_H
#define PLANE_H

#include "airfoil.h"
#include "vec2.h"
#include "engine.h"
#include "wheels.h"

class Plane {
public:
    Plane(Vec2 pos,
          Vec2 velocity,
          double angle,
          double mass,
          double inertia,

          Airfoil wings,
          Vec2 wingsPoint,
          Airfoil elevators,
          Vec2 elevatorsPoint,
          Engine engine,
          Wheels frontWheels,
          Wheels mainWheels) :
          
          pos{pos},
          velocity{velocity},
          angle{angle},
          angularVelocity{0},
          mass{mass},
          inertia{inertia},
          wings{wings},
          wingsPoint{wingsPoint},
          elevators{elevators},
          elevatorsPoint{elevatorsPoint},
          engine{engine},
          frontWheels{frontWheels},
          mainWheels{mainWheels}
    {}

    Plane(const Plane& other) = default;
    Plane(Plane&& other) = default;

    double getHeight() const {return pos.getY();}
    const Vec2& getPos() const {return pos;}
    void update(double delta);

    int getThrottle() const {return engine.getThrottle();}
    void setThrottle(int throttle) {engine.setThrottle(throttle);}

    double getElevatorDeflection() const {return elevators.getDeflection();}
    void setElevatorDeflection(double deflection) {elevators.setDeflection(deflection);}

    void toggleFlaps() {
        wings.toggleFlaps();
    }
    void toggleBrakes() {
        mainWheels.toggleBrakes();
        frontWheels.toggleBrakes();
    }

    static Plane getDefaultPlane();

private:
    Vec2 pos; // For now, xy coordinates of the plane
    Vec2 velocity;
    double angle; // angle of plane relative to horizon
    double angularVelocity;
    double mass;
    double inertia;

    Vec2 wingsPoint; // distance from wingss' force to the mass center
    Airfoil wings;

    Vec2 elevatorsPoint;
    Airfoil elevators;

    Engine engine;

    Wheels frontWheels;
    Wheels mainWheels;
};


#endif
