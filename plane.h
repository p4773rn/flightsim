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
          Airfoil elevators0,
          Airfoil elevators1,
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
          elevators0{elevators0},
          elevators1{elevators1},
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

    double getElevatorDeflection() const {return elevatorDeflection;}
    void setElevatorDeflection(double deflection);

    double getElevatorAngleOffset() const {return elevators0.getAirfoilAngleOffset();}
    void setElevatorAngleOffset(double angle);

    void toggleBreaks() {
        mainWheels.toggleBreaks();
        frontWheels.toggleBreaks();
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
    Airfoil elevators0; // elevators without flaps
    Airfoil elevators1; // elevators with flaps
    double elevatorDeflection = 0; // interpolate between with and without flaps

    Engine engine;

    Wheels frontWheels;
    Wheels mainWheels;
};


#endif
