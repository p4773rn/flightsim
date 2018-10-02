#include "plane.h"
#include "environment.h"

void Plane::update(double delta) {
    Vec2 netForce;
    
    netForce += Vec2(0, GRAVITATIONAL_ACCELERATION * mass);
    
    netForce += wing.getForce(velocity, angle, getAirPressure(pos.getY()));

    netForce += engine.getThrust(angle);

    //velocity += netForce / mass * delta;
    pos += velocity * delta;
}
