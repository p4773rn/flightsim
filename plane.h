#ifndef PLANE_H
#define PLANE_H

#include "airfoil.h"
#include "vec2.h"
#include "engine.h"

class Plane {
public:
    Plane() :
        engine(1000),
        wing(Table(
            {
                { -4.0 * M_PI / 180,  -0.14,   0.014,  0,   -0.036 },
                { -2.0 * M_PI / 180,   0.02,   0.008,  0,   -0.052 },
                {  0.0 * M_PI / 180,   0.14,   0.008,  0,   -0.090 },
                {  2.0 * M_PI / 180,   0.32,   0.012,  0,   -0.130 },
                {  4.0 * M_PI / 180,   0.46,   0.020,  0,   -0.160 },
                {  6.0 * M_PI / 180,   0.60,   0.030,  0,   -0.200 },
                {  8.0 * M_PI / 180,   0.76,   0.044,  0,   -0.240 },
                { 10.0 * M_PI / 180,   0.90,   0.060,  0,   -0.280 },
                { 12.0 * M_PI / 180,   1.04,   0.070,  0,   -0.310 },
                { 14.0 * M_PI / 180,   1.16,   0.096,  0,   -0.330 },
                { 15.0 * M_PI / 180,   1.22,   0.110,  0,   -0.340 },
                { 16.0 * M_PI / 180,   1.16,   0.140,  0,   -0.350 },
                { 18.0 * M_PI / 180,   1.02,   0.210,  0,   -0.384 },
                { 20.0 * M_PI / 180,   0.94,   0.260,  0,   -0.390 }
            }),
            20,
            0
        )
    {
        pos = Vec2(0, 100);
        velocity = Vec2(0, 0);
    }

    double getHeight() const {return pos.getY();}
    void update(double delta);

private:
    Vec2 pos; // For now, xy coordinates of the plane
    Vec2 velocity;
    double angle = 0; // angle of plane relative to horizon
    double angularVel = 0;

    double mass = 1000;

    Airfoil wing;
    Engine engine;
};


#endif
