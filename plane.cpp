#include "plane.h"


void Plane::update(float delta) {
    Vec2 netForce;
    
    netForce += Vec2(0, -9.8 * mass);
    
    netForce += wing.getLift(velocity, angle);

    
    velocity += netForce / mass * delta;
    pos += velocity * delta;
}
