#ifndef PLANE_H
#define PLANE_H

#include "airfoil.h"
#include "vec2.h"

class Plane {
public:
    Plane() {
        pos = Vec2(0, 100);
        velocity = Vec2(0, 0);
    }

    float getHeight() const {return pos.getY();}
    void update(float delta);

private:
    Vec2 pos; // For now, xy coordinates of the plane
    Vec2 velocity;
    float angle = 0; // angle of plane relative to horizon
    float angularVel = 0;

    float mass = 1000;

    Airfoil wing;
};


#endif
